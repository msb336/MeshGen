

#include "GeneratorBase.h"

GeneratorBase::GeneratorBase(const GeneratorBase::ConfigurationSettings* configuration_settings)
{
	std::string line;
	std::ifstream objfile(configuration_settings->config_file);
	procedural_parameters_->file_location =
		configuration_settings->config_file.substr(0, configuration_settings->config_file.find_last_of("\\/"));
#ifdef WIN32
	procedural_parameters_->file_location += "\\";
#else
	procedural_parameters_->file_location += "/";
#endif


	while (getline(objfile, line))
	{
		std::stringstream ss(line);
		std::string type;
		ss >> type;
		if (type == "file_location:") { ss >> procedural_parameters_->file_location; }
		if (type == "x_scale:") { ss >> procedural_parameters_->scale_x; }
		if (type == "y_scale:") { ss >> procedural_parameters_->scale_y; }
		if (type == "rows:") { ss >> procedural_parameters_->rows; }
		if (type == "columns:") { ss >> procedural_parameters_->columns; }
		if (type == "tile_size:") { ss >> procedural_parameters_->tile_size; }
		if (type == "load_adjacency:") { ss >> procedural_parameters_->load_adjacency; }
		if (type == "render_adjacency:") { ss >> procedural_parameters_->render_adjacency; }
	}
	procedural_mesh_.resize(procedural_parameters_->rows * procedural_parameters_->columns);
}


void GeneratorBase::generateAndUnloadTiles(const RenderingActions& rendering_action)
{
	loadTilesAsync(rendering_action.load);
	unloadTilesAsync(rendering_action.unload);
}
void GeneratorBase::loadTilesAsync(const TileList& tiles_to_load)
{

}


void GeneratorBase::unloadTilesAsync(const TileList& tiles_to_unload)
{
	for (auto tile : tiles_to_unload)
		procedural_mesh_[tile].clear();
}


Tile2d GeneratorBase::tileIndexToSubscript(const int& index)
{
	return Tile2d(index % procedural_parameters_->rows, int(float(index) / float(procedural_parameters_->rows)));
}

int GeneratorBase::tileSubscriptToIndex(const Tile2d& tile)
{
	return int{ tile.x + procedural_parameters_->rows * tile.y };
}

Position GeneratorBase::tileIndexToPosition(const int& index)
{
	Tile2d tile = tileIndexToSubscript(index);
	return Position(float(tile.y)*procedural_parameters_->tile_size*procedural_parameters_->scale_y, 
		float(tile.x)*procedural_parameters_->tile_size*procedural_parameters_->scale_x, 0);
}
