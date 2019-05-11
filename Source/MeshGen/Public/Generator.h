// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CommonStructs.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

UCLASS()
class MESHGEN_API AGenerator : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void loadMesh(int index);



	std::set<int> whichTiles();
	void determineLoadingAction(const std::set<int>& desired_tiles,
								std::set<int>& unbuilt_tiles,
								std::set<int>& obscelete_tiles);
	void loadTiles(const std::set<int>& tiles_to_build);
	void renderMeshSection(const int& section);
	void deconstructMeshSection(const int& section);

	void getPlayerLocation();
	void update();



private:
	UPROPERTY() UProceduralMeshComponent* procedural_mesh_component_;
	UPROPERTY() UMaterialInterface* material_;

private:
	int row_count_{ 0 }, column_count_{ 0 };
	MeshParameters parameters_;
	std::array<float, 2> bounds_{ 1500, 1500 };
	Position player_location_{ 0,0,0 };
	std::set<int> loaded_tiles_;
	ProceduralMesh procedural_mesh_;
	std::string mesh_location_{ "C:\\Users\\v-mattbr\\repos\\unreal-envs\\ProceduralMesh\\Plugins\\MeshGen\\Content\\MeshComponents\\" };
	std::string filename_{ "C:\\Users\\v-mattbr\\repos\\unreal-envs\\ProceduralMesh\\Plugins\\MeshGen\\Content\\MeshComponents\\meshinfo" };
	
	std::array<int, 2> player_tile_;
};
