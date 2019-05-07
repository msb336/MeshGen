// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator.h"


// Sets default values
AGenerator::AGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
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
	std::string loader_text = loadMesh();
	return FString(loader_text.c_str());
}

std::array<float, 2> AGenerator::distanceFromBoundary(const FVector& position)
{
	std::array<float, 2> distance{ bounds_[0] - position.Component(0), bounds_[1] - position.Component(1) };
	return distance;
}

FString AGenerator::update(const FVector& player_position)
{
	std::string loadtext("");
	auto distance = distanceFromBoundary(player_position);
	if (distance[0] < 1000) {
		column_count_++;
		loadtext=loadMesh();
	}
	else if(distance[1] < 1000) {
		row_count_++;
		loadtext=loadMesh();
	}
	loadtext += " player position: " + std::to_string(player_position.Component(0)) + " " + std::to_string(player_position.Component(1)) + " new distance: " + std::to_string(distance[0]) + " " + std::to_string(distance[1]);
	return  FString(loadtext.c_str());
}


std::string AGenerator::loadMesh()
{
	std::string filename = mesh_location_ + "image_mesh" + std::to_string(row_count_) + "_" + std::to_string(column_count_) + ".obj";
	std::string line;
	std::ifstream objfile(filename);
	std::string return_text = std::string("No file named ") + filename;

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
		return_text = std::string("found file. ") + " boundary: " + std::to_string(max_x) + " " + std::to_string(max_y);
		bounds_[0] = max_x;
		bounds_[1] = max_y;
	}
	
	section_count_++;
	return return_text;
}