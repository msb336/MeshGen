// Fill out your copyright notice in the Description page of Project Settings.

#include "Generator.h"


// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//vertices.Empty();
	//vertices_.Add(FVector(0, 0, 0));
	//vertices_.Add(FVector(square_dimensions_, 0, 0));
	//vertices_.Add(FVector(square_dimensions_, square_dimensions_, 0));
	//vertices_.Add(FVector(0, square_dimensions_, 0));

	////index_.Empty();
	//index_.Add(0);
	//index_.Add(1);
	//index_.Add(3);
	//index_.Add(1);
	//index_.Add(2);
	//index_.Add(3);
	loadMesh("maze.obj");

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

bool AGenerator::update()
{
	counter_++;
	generateNewPoints();
	return true;
}

void AGenerator::generateNewPoints()
{
	//float corner = float(counter_) * square_dimensions_;
	//int start_index = 4 * counter_;
	//vertices_.Add(FVector(corner, corner, 0));
	//vertices_.Add(FVector(corner + square_dimensions_, corner, 0));
	//vertices_.Add(FVector(corner + square_dimensions_, corner + square_dimensions_, 0));
	//vertices_.Add(FVector(corner, corner + square_dimensions_, 0));

	////index_.Empty();
	//index_.Add(start_index);
	//index_.Add(start_index + 1);
	//index_.Add(start_index + 3);
	//index_.Add(start_index + 1);
	//index_.Add(start_index + 2);
	//index_.Add(start_index + 3);

}

void AGenerator::loadMesh(const std::string& filename)
{
	std::string line;
	std::ifstream objfile("maze.obj");
	while (getline(objfile, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;                  //and read the first whitespace-separated token

		std::array<float, 3> vertex;
		int index;
		if (type == "v")
		{
			for (int i = 0; i < 3; i++)
			{
				ss >> vertex[i];
			}
			vertices_.Add(FVector(vertex[0], vertex[1], vertex[2]));
		}
		else if (type == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				ss >> index;
				index_.Add(index);
			}
		}
	}
}
