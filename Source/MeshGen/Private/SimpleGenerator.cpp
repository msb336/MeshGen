#include "SimpleGenerator.h"

SimpleGenerator::SimpleGenerator(const GeneratorBase::ConfigurationSettings* configuration_settings)
{
	procedural_parameters_->scale_x = 50;
	procedural_parameters_->scale_y = 50;
	procedural_parameters_->rows = 1000;
	procedural_parameters_->columns = 1000;
	procedural_parameters_->tile_size = 20;
	procedural_parameters_->load_adjacency = 2;
	procedural_parameters_->render_adjacency = 1;
	procedural_mesh_.resize(procedural_parameters_->rows * procedural_parameters_->columns);
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
			Position relative_position{ float(i)*procedural_parameters_->scale_x, float(j)*procedural_parameters_->scale_y, 0 }; //, sineWave(i, j) * 1000
			auto new_point = start_position + relative_position;
			new_point.z = 1000*sineWave(new_point.x, new_point.y);
			UE_LOG(LogTemp, Log, TEXT("new point: (%f,%f,%f)"), new_point.x, new_point.y, new_point.z);
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
			if ( (iterator % useful_size != useful_size - 1) 
				&& (iterator + useful_size+1 < useful_size*useful_size) )
			{
				for (auto connection : connection_list)
					new_mesh_section.triangle_list.Add(connection + iterator);
			}
			iterator++;
		}
	}


	UE_LOG(LogTemp, Log, TEXT("New mesh has %d vertices and %d indices"), new_mesh_section.vertex_list.Num(), new_mesh_section.triangle_list.Num());
	return new_mesh_section;
}

float SimpleGenerator::random()
{
	return float(rand()) / float(RAND_MAX);
}

float SimpleGenerator::sineWave(const float& i, const float& j)
{
	return sin(i/1000) + cos(j/1000);
}