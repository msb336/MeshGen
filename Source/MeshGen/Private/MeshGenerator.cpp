// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

// Sets default values
AMeshGenerator::AMeshGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/MeshGen/generated_material.generated_material'"));
	
	material_ = MaterialOb.Object; 

	procedural_mesh_component_ = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	procedural_mesh_component_->bUseAsyncCooking = true;
	procedural_mesh_component_->SetupAttachment(RootComponent);

	loadFromConfig();

}

void AMeshGenerator::loadFromConfig()
{
	std::string config{ "" }, generator_type{ "EMPTY" };
	std::string line;
	std::string configuration_fullpath = TCHAR_TO_UTF8(*FPaths::ProjectConfigDir()) + configuration_file_;
	std::ifstream objfile(configuration_fullpath);
	while (getline(objfile, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "settings_file:") 
		{
			ss >> config;
		}
		if (type == "generator_type:") 
		{
			ss >> generator_type;
		}
	}
	configuration_settings_ = new GeneratorBase::ConfigurationSettings{ generator_type, config };
	if (configuration_settings_->generator_type != GeneratorBase::GENERATORTYPE::EMPTY) {
		generator_ = generator_factory_->createGenerator(configuration_settings_);
	}

}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	//getPlayerLocation();
	//auto render_actions = getRenderingActions();
	//render_actions.load = render_actions.render;
	//generator_->generateAndUnloadTiles(render_actions);
	
}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Determine Location
	update();


}

void AMeshGenerator::update()
{
	// Determine Location
	getPlayerLocation();

	// Determine which tiles to load into memory and render
	auto render_actions = getRenderingActions();

	// tell generator to generate/load new tiles + unload obsolete tiles
	generator_->generateAndUnloadTiles(render_actions);

	// render new tiles
	renderTilesAsync(render_actions.render);

	// derender old tiles
	derenderTilesAsync(render_actions.derender);
}


void AMeshGenerator::getPlayerLocation()
{
	auto position = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	player_location_ = Position{ position.Component(0), position.Component(1), position.Component(2) };
	player_tile_.x = int(player_location_.y / (generator_->procedural_parameters_->tile_size*generator_->procedural_parameters_->scale_x));
	player_tile_.y = int(player_location_.x / (generator_->procedural_parameters_->tile_size*generator_->procedural_parameters_->scale_y));
}

void AMeshGenerator::findAdjacentTiles(TileList& adjacent_tiles, const int& level_of_adjacency = 1)
{
	Tile2d east{ 1,0 }, west{ -1,0 }, north{ 0,1 }, south{ 0,-1 };
	Tile2d position = player_tile_ +Tile2d{ -level_of_adjacency, -level_of_adjacency };

	adjacent_tiles.insert(int{ player_tile_.x + generator_->procedural_parameters_->rows * player_tile_.y });
	for (auto dir : { east, north, west, south })
	{
		for (int i = -level_of_adjacency; i < level_of_adjacency; i++)
		{
			
			if (position.x >= 0 && position.x < generator_->procedural_parameters_->rows &&
				position.y >= 0 && position.y < generator_->procedural_parameters_->columns)
			{
				adjacent_tiles.insert(generator_->tileSubscriptToIndex(position));
			}
			position += dir;
		}
	}
	if (level_of_adjacency > 1)
	{
		findAdjacentTiles(adjacent_tiles, level_of_adjacency - 1);
	}

}

RenderingActions AMeshGenerator::getRenderingActions()
{
	// determine tiles to render and load into memory
	TileList render_zone, load_zone;
	findAdjacentTiles(render_zone, generator_->procedural_parameters_->render_adjacency);
	findAdjacentTiles(load_zone, generator_->procedural_parameters_->load_adjacency);

	// determine which tiles in memory are redundant
	RenderingActions render_actions;
	inverseIntersection(render_zone, rendered_tiles_, render_actions.render, render_actions.derender);
	inverseIntersection(load_zone, loaded_tiles_, render_actions.load, render_actions.unload);
	std::merge(render_actions.load.begin(), render_actions.load.end(), render_actions.render.begin(), render_actions.render.end(), std::inserter(render_actions.load, render_actions.load.end()));
	return render_actions;
}

void AMeshGenerator::renderTilesAsync(const TileList& tiles_to_render)
{
	for (auto tile : tiles_to_render)
	{
		if (tile >= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("rendering tile %d"), tile);


			if (generator_->procedural_mesh_[tile].populated)
			{
				procedural_mesh_component_->SetMaterial(tile, material_);
				procedural_mesh_component_->CreateMeshSection_LinearColor(tile,
					generator_->procedural_mesh_[tile].vertex_list,
					generator_->procedural_mesh_[tile].triangle_list,
					TArray < FVector >(),
					TArray < FVector2D >(),
					generator_->procedural_mesh_[tile].color_list,
					TArray < FProcMeshTangent >(),
					true);
				rendered_tiles_.insert(tile);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Tile in section %d is not loaded into memory"), tile);
			}
		}
	}
}

void AMeshGenerator::derenderTilesAsync(const TileList& tiles_to_derender)
{
	for (auto tile : tiles_to_derender)
	{
		//procedural_mesh_component_->ClearMeshSection(tile);

		//Super hacky
		procedural_mesh_component_->CreateMeshSection_LinearColor(tile,
			TArray<FVector>(),
			TArray<int32>(),
			TArray < FVector >(),
			TArray < FVector2D >(),
			TArray<FLinearColor>(),
			TArray < FProcMeshTangent >(),
			true);
		rendered_tiles_.erase(tile);
		
	}

}

void AMeshGenerator::inverseIntersection(const TileList& a, const TileList& b, TileList& unique_to_a, TileList& unique_to_b)
{
	std::set_difference(a.begin(), a.end(),
		b.begin(), b.end(),
		std::inserter(unique_to_a, unique_to_a.begin()));
	std::set_difference(b.begin(), b.end(),
		a.begin(), a.end(),
		std::inserter(unique_to_b, unique_to_b.begin()));
}




///Temporary
void AMeshGenerator::addWater(const FVector& location)
{
	//static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/Game/StartContent/Materials/M_Water_Ocean.M_Water_Ocean'"));
	//UStaticMeshComponent* new_water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	//UStaticMesh* static_mesh = 
	//new_water->SetStaticMesh()
	//new_water->SetupAttachment(RootComponent);

}