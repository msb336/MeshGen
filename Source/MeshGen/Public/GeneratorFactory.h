#pragma once
#include <memory>
#include "GeneratorBase.h"
#include "DEMConverter.h"

class GeneratorFactory
{
public:
	virtual std::unique_ptr<GeneratorBase> createGenerator(const GeneratorBase::ConfigurationSettings* generator_settings)
	{
		switch (generator_settings->generator_type) {
		case GeneratorBase::GENERATORTYPE::DEM:
			return std::unique_ptr<DEMConverter>(new DEMConverter(generator_settings));
			break;
		default:
			throw std::invalid_argument("ya shit's busted");
		}
	}
};

