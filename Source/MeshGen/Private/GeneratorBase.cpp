

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
