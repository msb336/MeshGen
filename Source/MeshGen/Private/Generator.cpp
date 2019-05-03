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
	counter_++;
	std::string loader_text = loadMesh("c:\\users\\v-mattbr\\repos\\unreal-envs\\ProceduralMesh\\Plugins\\MeshGen\\Content\\image_mesh.obj");
	return FString(loader_text.c_str());
}
std::array<float, 2> AGenerator::distanceFromBoundary(const FVector& position)
{
	std::array<float, 2> distance{ bounds_[0] - position.x, bounds_[1] - position.y };
	return distance;
}
void AGenerator::update(const FVector& player_position)
{
	auto distance = distanceFromBoundary(player_position);
	if (distance[0] < 50) && (distance[1] < 50) {
		//add diagonal + sides
	}
	else if (distance[0] < 50) {
		//add in x direction
	}
	else if (distance[1] < 50) {
		// add in y direction
	}
	else {
		update_mesh_ = false;
	}


}


void AGenerator::generateNewPoints()
{
}

std::string AGenerator::loadMesh(const std::string& filename)
{
	std::string line;
	std::ifstream objfile(filename);
	std::string return_text = std::string("No file named ") + filename;
	if (!gen_called_)
	{
		while (getline(objfile, line))
		{
			return_text = std::string("found file ") + filename;
			std::stringstream ss(line);
			std::string type;
			ss >> type;                  //and read the first whitespace-separated token

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
				//uvs_.Add(FVector2D(vertex[0], vertex[1]));
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
	}

	return return_text;
}