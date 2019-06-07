// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CommonStructs.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorBase.generated.h"

UCLASS()
class MESHGEN_API AGeneratorBase : public AActor
{
	GENERATED_BODY()

private:
	struct RenderingActions {
		TileList load;
		TileList render;
		TileList unload;
		TileList derender;
	};

public:	
	// Sets default values for this actor's properties
	AGeneratorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void loadTilesAsync(const TileList& tiles_to_load);
	virtual void getParametersFromConfig(const std::string& config_file);

private:
	void update();
	void getPlayerLocation();
	RenderingActions getRenderingActions();
	void findAdjacentTiles(TileList& adjacent_tiles, const int& level_of_adjacency);


	void renderTilesAsync(const TileList& tiles_to_render);
	void derenderTilesAsync(const TileList& tiles_to_render);
	void unloadTilesAsync(const TileList& tiles_to_load);


	void inverseIntersection(const TileList& a, const TileList& b, TileList& unique_to_a, TileList& unique_to_b);

protected:
	ProceduralParameters* procedural_parameters_;
	Position player_location_;
	Tile2d player_tile_;

	TileList loaded_tiles_;
	TileList rendered_tiles_;
	ProceduralMesh procedural_mesh_;

private:
	UPROPERTY() UProceduralMeshComponent* procedural_mesh_component_;
	UPROPERTY() UMaterialInterface* material_;

};
