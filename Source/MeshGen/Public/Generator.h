// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <fstream>
#include <sstream>
#include <string>

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
		Vertices vertices_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		UV uvs_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		Triangles index_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		bool update_mesh_ = false;

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
		void update(const FVector& player_position);
private:
	int counter_{ 0 };
	std::array<float, 2> bounds_{ 50, 50 };
	void generateNewPoints();
	std::string loadMesh(const std::string& );
	std::array<float, 2> distanceFromBoundary(const FVector&);

	std::vector<std::vector<TriangularMesh>> chunks;


	
	
};
