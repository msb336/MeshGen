// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator.h"


// Sets default values
AGenerator::AGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
	std::string line;
	std::ifstream objfile(filename);
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

	}

}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AGenerator::generateMesh()
{
	std::string loader_text = loadMesh(0,0,0);
	return FString(loader_text.c_str());
}

std::array<float, 2> AGenerator::distanceFromBoundary()
{
	std::array<float, 2> distance{ bounds_[0] - player_location_.x, bounds_[1] - player_location_.y };
	return distance;
}

void AGenerator::getPlayerLocation()
{
	auto position = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	player_location_ = Position{ position.Component(0), position.Component(1), position.Component(2) };
}

TileSet AGenerator::whichTiles()
{
	// which tile is player on?
	player_tile = TileArray{ int( player_location_.y / parameters_.tile_size ), int( player_location_.x / parameters_.tile_size ) };

	// which tiles should be loaded?
	TileSet desired_tiles;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1;j <= 1;j++) {
			TileArray new_tile{ player_tile[0] + i,player_tile[1] + j };
			if (new_tile[0] >= 0 && new_tile[0] < parameters_.rows &&
				new_tile[1] >= 0 && new_tile[1] < parameters_.columns)
			{
				desired_tiles.insert(Tile{new_tile[0]+parameters_.rows * new_tile[1]});
			}
		}
	}
	return desired_tiles;
}

void determineLoadingAction(const TileSet& desired_tiles,
	TileSet& unbuilt_tiles,
	TileSet& obscelete_tiles)
{
	// populate unbuilt tiles
	std::set_difference(desired_tiles.begin(), desired_tiles.end(), 
		loaded_tiles_.begin(), loaded_tiles_.end(), 
		std::inserter(unbuilt_tiles, unbuilt_tiles.begin()));

	// populate obscelete tiles
	std::set_difference(loaded_tiles_.begin(), loaded_tiles_.end(), 
		desired_tiles.begin(), desired_tiles.end(),
		std::inserter(unbuilt_tiles, unbuilt_tiles.begin()));
}

void loadTiles(const TileSet& tiles_to_build)
{
	for (TileSet::const_iterator index = tiles_to_build.begin(); index != tiles_to_build.end(); ++index) {
		int i = *index % rowsize;
		int j = int ( *index / rowsize );
		loadMesh(i, j, index);
	}
}
void deleteTile(const Tile& tile_to_delete)
{
	// Should be blueprint implementable
	return tile_to_delete;
}

void AGenerator::update()
{
	// No longer needs to be blueprint callable (occurs in tick override)
	////std::string loadtext("");
	getPlayerLocation();
	auto distance = distanceFromBoundary();
	TileSet desired_tiles = whichTiles();


	TileSet tiles_to_delete, tiles_to_load;
	determineLoadingAction(desired_tiles, tiles_to_load, tiles_to_delete);
	loadTiles(tiles_to_load);
	for (TileSet::const_iterator index = tiles_to_delete.begin(); index != tiles_to_delete.end(); ++index) {
		deleteTile(index);
	}

	//if (distance[0] < 1000) {
	//	column_count_++;
	//	loadtext=loadMesh();
	//}
	//else if(distance[1] < 1000) {
	//	row_count_++;
	//	loadtext=loadMesh();
	//}

	//loadtext += " player position: " + std::to_string(player_position.Component(0)) + " " + std::to_string(player_position.Component(1)) + " new distance: " + std::to_string(distance[0]) + " " + std::to_string(distance[1]);
	//return  FString(loadtext.c_str());
}


void AGenerator::loadMesh(int row, int column, int index)
{
	// Should be able to execute in blueprint
	std::string local_name = "image_mesh" + std::to_string(row) + "_" + std::to_string(column) + ".obj";
	std::string filename = mesh_location_ + local_name;

	std::string line;
	std::ifstream objfile(filename);
	//std::string return_text = std::string("No file named ") + filename;

	vertices_.Empty();
	index_.Empty();
	uvs_.Empty();

	float max_x{ 0 }, max_y{ 0 };

	while (getline(objfile, line))
	{
		
		std::stringstream ss(line);
		std::string type;
		ss >> type;

		std::array<float, 3> vertex;
		std::array<float, 2> uv;
		int index;
		if (type == "v")
		{
			for (int i = 0; i < 3; i++)
			{
				ss >> vertex[i];
			}
			vertices_.Add(FVector(vertex[0], vertex[1], vertex[2]));
			if (vertex[0] > max_x) { max_x = vertex[0]; }
			if (vertex[1] > max_y) { max_y = vertex[1]; }
		}
		else if (type == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				ss >> index;
				index_.Add(index);
			}
		}
		else if (type == "u")
		{
			for (int i = 0; i < 2; i++)
			{
				ss >> uv[i];
					
			}
			uvs_.Add(FVector2D(uv[0], uv[1]));
		}

	}

	boundary_ = FVector2D(max_x, max_y);
	if (max_x || max_y){ 
		update_mesh_ = true; 
		//return_text = std::string("loading ") + local_name + " boundary: " + std::to_string(max_x) + " " + std::to_string(max_y);
		bounds_[0] = max_x;
		bounds_[1] = max_y;
	}
	
	//section_count_++;
	//return return_text;
}



virtual void Tick(float DeltaTime) override
{
	update();
	
}