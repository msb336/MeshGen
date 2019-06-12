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
		SIMPLE,
		EMPTY
	};
	struct ConfigurationSettings
	{
		std::string config_file;
		GeneratorBase::GENERATORTYPE generator_type = GeneratorBase::GENERATORTYPE::EMPTY;
		ConfigurationSettings(std::string enum_type, std::string file_location = "") : config_file{ file_location }
		{
			if (enum_type == "DEM")				generator_type = GENERATORTYPE::DEM;
			else if (enum_type == "PROCEDURAL") generator_type = GENERATORTYPE::PROCEDURAL;
			else if (enum_type == "IMAGE")		generator_type = GENERATORTYPE::IMAGE;
			else if (enum_type == "GAN")		generator_type = GENERATORTYPE::GAN;
			else if (enum_type == "SIMPLE")		generator_type = GENERATORTYPE::SIMPLE;
			else								generator_type = GENERATORTYPE::EMPTY;
		}
	};

public:	
	// Sets default values for this actor's properties
	GeneratorBase();
	Tile2d tileIndexToSubscript(const int& index);
	int tileSubscriptToIndex(const Tile2d& tile);
	Position tileIndexToPosition(const int& index);

public:	
	virtual void loadTilesAsync(const TileList& tiles_to_load);
	void generateAndUnloadTiles(const RenderingActions& rendering_action);
	ProceduralParameters* procedural_parameters_ = new ProceduralParameters;
	ProceduralMesh procedural_mesh_;

private:
	void unloadTilesAsync(const TileList& tiles_to_load);

};
