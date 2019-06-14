#pragma once
#include "GeneratorBase.h"
#include "math.h"
#include "PerlinNoise.hpp"

class SimpleGenerator : public GeneratorBase
{
public:
	SimpleGenerator(const GeneratorBase::ConfigurationSettings* config_file) : GeneratorBase(config_file) {};

private:
	virtual void loadTilesAsync(const TileList& tiles) override;
	float random();
	float sineWave(const Position& position);
	float perlin( const Position& position );
	MeshSection createSomeShit(const int& start_position);

private:
	siv::PerlinNoise perlin_noise_{};
};