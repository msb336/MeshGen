#pragma once
#include "GeneratorBase.h"
#include "math.h"
#include "PerlinNoise.hpp"

class SimpleGenerator : public GeneratorBase
{
private: 
	struct PerlinParameters {
		float amplitude{ 1 };
		float frequency{ (float) exp(1) };
		int octaves{ 1 };
		int max_oct_modifier{ 1 };
	};

public:
	SimpleGenerator(const GeneratorBase::ConfigurationSettings* config_file); // : GeneratorBase(config_file) {};

private:
	virtual void loadTilesAsync(const TileList& tiles) override;
	float random();
	float sineWave(const Position& position);
	float perlin( const Position& position, const int& amp_modifier=1 );
	MeshSection createSomeShit(const int& start_position);

private:
	siv::PerlinNoise perlin_noise_{};
	PerlinParameters* perlin_parameters_ = new PerlinParameters;
};