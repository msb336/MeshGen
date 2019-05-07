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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		TArray<FVector> vertices_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		TArray<FVector2D> uvs_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		TArray<int32> index_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		bool update_mesh_ = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		int section_count_{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		FVector2D boundary_;

public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Procedural Generation")
		FString generateMesh();
	UFUNCTION(BlueprintCallable, Category = "Procedural Generation")
		FString update(const FVector& player_position);
private:
	int row_count_{ 0 }, column_count_{ 0 };
	std::array<float, 2> bounds_{ 1500, 1500 };
	void generateNewPoints();
	std::string loadMesh( );
	std::array<float, 2> distanceFromBoundary(const FVector&);

	//std::vector<std::vector<TriangularMesh>> chunks;
	std::string mesh_location_{ "C:\\Users\\v-mattbr\\repos\\unreal-envs\\ProceduralMesh\\Plugins\\MeshGen\\Content\\MeshComponents\\" };


	
	
};
