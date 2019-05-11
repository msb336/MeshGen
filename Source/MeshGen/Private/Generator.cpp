// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator.h"


// Sets default values
AGenerator::AGenerator()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/MeshGen/generated_material.generated_material'"));
	material_ = MaterialOb.Object;

	procedural_mesh_component_ = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	procedural_mesh_component_->bUseAsyncCooking = true;
	procedural_mesh_component_->SetupAttachment(RootComponent);
	

	PrimaryActorTick.bCanEverTick = true;
	std::string line;
	std::ifstream objfile(filename_);
	while (getline(objfile, line))
	{

		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "x_scale:") { ss >> parameters_.scale_x; }
		if (type == "y_scale:") { ss >> parameters_.scale_y; }
		if (type == "rows:") { ss >> parameters_.rows; }
		if (type == "columns:") { ss >> parameters_.columns; }
		if (type == "chunk_size:") { ss >> parameters_.tile_size; }

		procedural_mesh_.resize(parameters_.rows * parameters_.columns);

	}



}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	//renderMeshSection(0);
}


void AGenerator::deconstructMeshSection(const int& section)
{
	UE_LOG(LogTemp, Log, TEXT("clearing section %d"), section);
	procedural_mesh_component_->ClearMeshSection(section);
}
void AGenerator::renderMeshSection(const int& section)
{
	UE_LOG(LogTemp, Log, TEXT("loading section %d"), section);
	if (procedural_mesh_[section].vertex_list.Num() < 1)
	{
		loadMesh(section);
	}

	//UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(material_, this);
	procedural_mesh_component_->SetMaterial(section, material_);

	procedural_mesh_component_->CreateMeshSection_LinearColor(section,
		procedural_mesh_[section].vertex_list,
		procedural_mesh_[section].triangle_list,
		//procedural_mesh_[section].normal_list,
		TArray < FVector > (),
		//procedural_mesh_[section].uv,
		TArray < FVector2D >(),
		procedural_mesh_[section].color_list,
		//procedural_mesh_[section].tangents, 
		TArray < FProcMeshTangent > (),
		true);
	

}

void AGenerator::getPlayerLocation()
{
	auto position = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	player_location_ = Position{ position.Component(0), position.Component(1), position.Component(2) };
}

std::set<int> AGenerator::whichTiles()
{
	// which tile is player on?
	player_tile_ = std::array<int, 2>{ int( player_location_.y / (parameters_.tile_size*parameters_.scale_x) ), int( player_location_.x / (parameters_.tile_size*parameters_.scale_y) ) };

	// which tiles should be loaded?
	std::set<int> desired_tiles;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1;j <= 1;j++) {
			std::array<int, 2> new_tile{ player_tile_[0] + i,player_tile_[1] + j };
			if (new_tile[0] >= 0 && new_tile[0] < parameters_.rows &&
				new_tile[1] >= 0 && new_tile[1] < parameters_.columns)
			{
				desired_tiles.insert(int{new_tile[0]+parameters_.rows * new_tile[1]});
			}
		}
	}

	//if (player_tile_[0] >= 0 && player_tile_[0] < parameters_.rows &&
	//	player_tile_[1] >= 0 && player_tile_[1] < parameters_.columns)
	//{
	//	desired_tiles.insert(int{ player_tile_[0]+parameters_.rows * player_tile_[1]});
	//}
	return desired_tiles;
}

void AGenerator::determineLoadingAction(const std::set<int>& desired_tiles,
	std::set<int>& unbuilt_tiles,
	std::set<int>& obscelete_tiles)
{
	// populate unbuilt tiles
	std::set_difference(desired_tiles.begin(), desired_tiles.end(), 
		loaded_tiles_.begin(), loaded_tiles_.end(), 
		std::inserter(unbuilt_tiles, unbuilt_tiles.begin()));

	// populate obscelete tiles
	std::set_difference(loaded_tiles_.begin(), loaded_tiles_.end(), 
		desired_tiles.begin(), desired_tiles.end(),
		std::inserter(obscelete_tiles, obscelete_tiles.begin()));


	for (std::set<int>::const_iterator index = unbuilt_tiles.begin(); index != unbuilt_tiles.end(); ++index) 
	{
		UE_LOG(LogTemp, Warning, TEXT("wants to load tile %d"), *index);
	}

	for (std::set<int>::const_iterator index = obscelete_tiles.begin(); index != obscelete_tiles.end(); ++index)
	{
		UE_LOG(LogTemp, Warning, TEXT("wants to delete tile %d"), *index);
	}
	loaded_tiles_ = desired_tiles;
}

void AGenerator::loadTiles(const std::set<int>& tiles_to_build)
{
	//std::string debug_output{ "loaded tiles:" };
	for (std::set<int>::const_iterator index = tiles_to_build.begin(); index != tiles_to_build.end(); ++index) {
		int i = *index % parameters_.rows;
		int j = int ( *index / parameters_.rows);
		renderMeshSection(*index);
		//debug_output += " " + std::to_string(*index);
	}
	//UE_LOG(LogTemp, Warning, TEXT(debug_output.c_str()));

}

void AGenerator::update()
{
	// Get Player Location
	getPlayerLocation();

	// Determine which sections should be in the scene
	std::set<int> desired_tiles = whichTiles();

	// Determine which sections must be loaded / unloaded
	std::set<int> tiles_to_delete, tiles_to_load;
	determineLoadingAction(desired_tiles, tiles_to_load, tiles_to_delete);

	// Load new tiles
	loadTiles(tiles_to_load);

	// delete old tiles
	for (std::set<int>::const_iterator index = tiles_to_delete.begin(); index != tiles_to_delete.end(); ++index) {
		deconstructMeshSection(*index);
	}
	//UE_LOG(LogTemp, Warning, TEXT(debug_output.c_str()));

}


void AGenerator::loadMesh(int one_dimensional_index)
{
	// Should be able to execute in blueprint
	std::string local_name = "image_mesh" + std::to_string(one_dimensional_index) + ".obj";  // +"_" + std::to_string(column) + ".obj";
	std::string filename = mesh_location_ + local_name;

	std::string line;
	std::ifstream objfile(filename);
	//std::string return_text = std::string("No file named ") + filename;

	procedural_mesh_[one_dimensional_index].vertex_list.Empty();
	procedural_mesh_[one_dimensional_index].triangle_list.Empty();
	procedural_mesh_[one_dimensional_index].color_list.Empty();

	float max_x{ 0 }, max_y{ 0 };

	while (getline(objfile, line))
	{
		
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		std::array<float, 4> color;
		std::array<float, 3> vertex;
		std::array<float, 2> uv;
		int index;
		if (type == "v")
		{
			for (int i = 0; i < 3; i++)
			{
				ss >> vertex[i];
			}
			procedural_mesh_[one_dimensional_index].vertex_list.Add(FVector(vertex[0], vertex[1], vertex[2]));
			if (vertex[0] > max_x) { max_x = vertex[0]; }
			if (vertex[1] > max_y) { max_y = vertex[1]; }

			//Placeholder for actual datawrite:
			procedural_mesh_[one_dimensional_index].normal_list.Add(FVector(0, 0, 1));
			
			
			procedural_mesh_[one_dimensional_index].tangents.Add(FProcMeshTangent(0, 0, 0));



		}
		else if (type == "c")
		{
			for (int i = 0; i < 4; i++)
			{ ss >> color[i]; }
			procedural_mesh_[one_dimensional_index].color_list.Add(FLinearColor(color[0], color[1], color[2], color[3]));
			//procedural_mesh_[one_dimensional_index].color_list.Add(FLinearColor::Red);
		}
		else if (type == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				ss >> index;
				procedural_mesh_[one_dimensional_index].triangle_list.Add(index);
			}
		}
		else if (type == "u")
		{
			for (int i = 0; i < 2; i++)
			{
				ss >> uv[i];
			}
			procedural_mesh_[one_dimensional_index].uv.Add(FVector2D(uv[0], uv[1]));
		}

	}

}



void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	update();
	
}