// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GeneratorBase.h"

class DEMConverter : public GeneratorBase
{
public:
	DEMConverter(const GeneratorBase::ConfigurationSettings* config_file) : GeneratorBase(config_file) {};

private:
	virtual void loadTilesAsync(const TileList&) override;
};
