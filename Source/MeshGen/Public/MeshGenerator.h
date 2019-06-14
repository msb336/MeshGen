// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GeneratorBase.h"
#include "GeneratorFactory.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshGenerator.generated.h"

UCLASS()
class MESHGEN_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshGenerator();

private:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void update();
	void loadFromConfig();
	void getPlayerLocation();
	void findAdjacentTiles(TileList& adjacent_tiles, const int& level_of_adjacency);
	RenderingActions getRenderingActions();

	void renderTilesAsync(const TileList& tiles_to_render);
	void derenderTilesAsync(const TileList& tiles_to_derender);
	void inverseIntersection(const TileList& a, const TileList& b, TileList& unique_to_a, TileList& unique_to_b);

	//temporary
	void addWater(const FVector& location);

private:
	std::unique_ptr<GeneratorBase>  generator_;
	GeneratorFactory* generator_factory_ = new GeneratorFactory;
	std::string configuration_file_ = "MeshGen.ini";
	GeneratorBase::ConfigurationSettings* configuration_settings_;

	Position player_location_;
	Tile2d player_tile_;

	TileList loaded_tiles_;
	TileList rendered_tiles_;
	

private:
	UPROPERTY() UProceduralMeshComponent* procedural_mesh_component_;
	UPROPERTY() UMaterialInterface* material_;

};