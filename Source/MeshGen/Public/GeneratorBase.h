// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CommonStructs.h"

class GeneratorBase
{
public:
	enum GENERATORTYPE
	{
		DEM,
		PROCEDURAL,
		IMAGE,
		GAN,
		EMPTY
	};
	struct ConfigurationSettings
	{
		std::string config_file;
		GeneratorBase::GENERATORTYPE generator_type = GeneratorBase::GENERATORTYPE::EMPTY;
	};

public:	
	// Sets default values for this actor's properties
	GeneratorBase();

public:	
	virtual void loadTilesAsync(const TileList& tiles_to_load);
	void generateAndUnloadTiles(const RenderingActions& rendering_action);
	ProceduralParameters* procedural_parameters_ = new ProceduralParameters;
	ProceduralMesh procedural_mesh_;

private:
	void unloadTilesAsync(const TileList& tiles_to_load);
	
	

};
