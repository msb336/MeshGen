#include "SimpleGenerator.h"


SimpleGenerator::SimpleGenerator(const GeneratorBase::ConfigurationSettings* config_settings) : GeneratorBase::GeneratorBase(config_settings)
{
	std::string line;
	std::ifstream objfile(config_settings->config_file);
	while (getline(objfile, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "amplitude:") { ss >> perlin_parameters_->amplitude; };
		if (type == "frequency:") { ss >> perlin_parameters_->frequency; };
		if (type == "octaves:") { ss >> perlin_parameters_->octaves; };
		if (type == "max_octave_modifier:") { ss >> perlin_parameters_->max_oct_modifier; };
	}

}
void SimpleGenerator::loadTilesAsync(const TileList& tiles)
{
	for (auto tile : tiles)
	{
		
		if (!procedural_mesh_[tile].populated)
		{
			procedural_mesh_[tile] = createSomeShit(tile);
			procedural_mesh_[tile].populated = true;
		}

	}
}


MeshSection SimpleGenerator::createSomeShit(const int& tile)
{
	Position start_position = tileIndexToPosition(tile);
	int useful_size = procedural_parameters_->tile_size + 1;
	std::array<int, 6> connection_list{ 0, 1, useful_size, 1, useful_size + 1, useful_size};
	MeshSection new_mesh_section;
	int iterator{ 0 };


	for (int i = 0; i < useful_size; i++)
	{
		for (int j = 0; j < useful_size; j++)
		{
			Position relative_position{ float(i)*procedural_parameters_->scale_x, float(j)*procedural_parameters_->scale_y, 0 };
			auto new_point = start_position + relative_position;

			float random_product{ random() * float(perlin_parameters_->max_oct_modifier) };
			int random_octave{ random_product > 1 ? int(random_product) : 1};

			new_point.z = perlin(new_point, random_octave);
			FLinearColor color{ new_point.z / 1000, 0, 1 - new_point.z / 1000 };

			if (j == 0)
			{
				color = FLinearColor::Red;
				if (tile % procedural_parameters_->columns != 0)
				{
					color = FLinearColor::Black;
					new_point.z = procedural_mesh_[tile - 1].populated ? 
						procedural_mesh_[tile - 1].vertex_list[useful_size - 1 + i*useful_size].Z : new_point.z;
				}
			}
			else if (i == 0)
			{
				color = FLinearColor::Yellow;
				if (tile >= procedural_parameters_->rows)
				{
					color = FLinearColor::Black;
					new_point.z = procedural_mesh_[tile - procedural_parameters_->rows].populated ? 
						procedural_mesh_[tile - procedural_parameters_->rows].vertex_list[useful_size*(useful_size-1) + j].Z : new_point.z;
				}
			}
			
			new_mesh_section.vertex_list.Add(FVector(new_point.x, new_point.y, new_point.z));
			new_mesh_section.color_list.Add(color);
			new_mesh_section.uv.Add(FVector2D(float(i)/float(useful_size), float(j)/float(useful_size)));

			if ( (iterator % useful_size != useful_size - 1) 
				&& (iterator + useful_size+1 < useful_size*useful_size) )
			{
				for (auto connection : connection_list)
					new_mesh_section.triangle_list.Add(connection + iterator);
			}
			iterator++;

		}

	}

	return new_mesh_section;
}

float SimpleGenerator::random()
{
	return float(rand()) / float(RAND_MAX);
}

float SimpleGenerator::sineWave(const Position& position)
{
	return sin((position.x/(1000*(1+0.5*random())))+random()) + cos((position.y/(1000*(1+0.5*random())))+random());
}


float SimpleGenerator::perlin(const Position& position, const int& amplitude_modifier)
{
	float freq = perlin_parameters_->frequency;
	float noise{  perlin_parameters_->amplitude * float(perlin_noise_.octaveNoise(freq*position.x, freq*position.y, perlin_parameters_->octaves * amplitude_modifier)) };
	return isnan(noise) ? 0 : noise;
}