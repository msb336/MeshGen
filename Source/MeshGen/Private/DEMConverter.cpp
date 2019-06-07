// Fill out your copyright notice in the Description page of Project Settings.


#include "DEMConverter.h"
void ADEMConverter::loadTilesAsync(const TileList& tiles_to_load)
{
	for (auto tile : tiles_to_load)
	{
		std::string local_name = "image_mesh" + std::to_string(tile) + ".obj";
		std::string filename = file_location_ + local_name;

		std::string line;
		std::ifstream objfile(filename);
		//std::string return_text = std::string("No file named ") + filename;

		procedural_mesh_[tile].clear();

		float max_x{ 0 }, max_y{ 0 };

		while (getline(objfile, line))
		{

			std::stringstream ss(line);
			std::string type;
			ss >> type;
			std::array<float, 4> color;
			std::array<float, 3> vertex;
			std::array<float, 2> uv;
			int index;
			if (type == "v")
			{
				for (int i = 0; i < 3; i++)
				{
					ss >> vertex[i];
				}
				procedural_mesh_[tile].vertex_list.Add(FVector(vertex[0], vertex[1], vertex[2]));
				if (vertex[0] > max_x) { max_x = vertex[0]; }
				if (vertex[1] > max_y) { max_y = vertex[1]; }

				//Placeholder for actual datawrite:
				procedural_mesh_[tile].normal_list.Add(FVector(0, 0, 1));
				procedural_mesh_[tile].tangents.Add(FProcMeshTangent(0, 0, 0));
			}
			else if (type == "c")
			{
				for (int i = 0; i < 4; i++)
				{
					ss >> color[i];
				}
				procedural_mesh_[tile].color_list.Add(FLinearColor(color[0], color[1], color[2], color[3]));
				//procedural_mesh_[tile].color_list.Add(FLinearColor::Red);
			}
			else if (type == "f")
			{
				for (int i = 0; i < 3; i++)
				{
					ss >> index;
					procedural_mesh_[tile].triangle_list.Add(index);
				}
			}
			else if (type == "u")
			{
				for (int i = 0; i < 2; i++)
				{
					ss >> uv[i];
				}
				procedural_mesh_[tile].uv.Add(FVector2D(uv[0], uv[1]));
			}

		}
	}
}