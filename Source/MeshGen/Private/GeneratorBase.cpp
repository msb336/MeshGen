

#include "GeneratorBase.h"

GeneratorBase::GeneratorBase()
{	
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
