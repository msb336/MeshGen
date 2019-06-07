// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GeneratorBase.h"
#include "DEMConverter.generated.h"

/**
 * 
 */
UCLASS()
class MESHGEN_API ADEMConverter : public AGeneratorBase
{
	GENERATED_BODY()


private:
	virtual void loadTilesAsync(const TileList& tiles_to_load) override;
	virtual void getParametersFromConfig(const std::string& config_file) override;

private :
	std::string file_location_;
};
