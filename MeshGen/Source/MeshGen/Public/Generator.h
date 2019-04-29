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
		TArray<FVector> vertices_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Generation")
		TArray<int32> index_;
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
	bool update();
private:
	int counter_{ 0 };
	float square_dimensions_ = 1000;
	void generateNewPoints();
	void loadMesh(const std::string& );


	
	
};
