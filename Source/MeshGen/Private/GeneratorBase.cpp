

#include "GeneratorBase.h"

AGeneratorBase::AGeneratorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOb(TEXT("Material'/MeshGen/generated_material.generated_material'"));
	material_ = MaterialOb.Object;

	procedural_mesh_component_ = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	procedural_mesh_component_->bUseAsyncCooking = true;
	procedural_mesh_component_->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AGeneratorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeneratorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	update();

}

void AGeneratorBase::update()
{
	// Determine Location
	getPlayerLocation();

	// Determine which tiles to load into memory and render
	auto render_actions = getRenderingActions();

	// load & render
	loadTilesAsync(render_actions.load);
	renderTilesAsync(render_actions.render);

	// unload & derender
	unloadTilesAsync(render_actions.unload);
	derenderTilesAsync(render_actions.derender);

}













/////// Think ///////////////////////
void AGeneratorBase::getPlayerLocation()
{
	auto position = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	player_location_ = Position{ position.Component(0), position.Component(1), position.Component(2) };
	player_tile_.x = int(player_location_.y / (procedural_parameters_->tile_size*procedural_parameters_->scale_x));
	player_tile_.y = int(player_location_.x / (procedural_parameters_->tile_size*procedural_parameters_->scale_y));
}

AGeneratorBase::RenderingActions AGeneratorBase::getRenderingActions()
{
	// determine tiles to render and load into memory
	TileList render_zone, load_zone;
	findAdjacentTiles(render_zone, procedural_parameters_->render_adjacency);
	findAdjacentTiles(load_zone, procedural_parameters_->load_adjacency);

	// determine which tiles in memory are redundant
	AGeneratorBase::RenderingActions render_actions;
	inverseIntersection(render_zone, rendered_tiles_, render_actions.render, render_actions.derender);
	inverseIntersection(load_zone, loaded_tiles_, render_actions.load, render_actions.unload);
	return render_actions;
}

void AGeneratorBase::findAdjacentTiles(TileList& adjacent_tiles, const int& level_of_adjacency = 1)
{
	Tile2d east{ 1,0 }, west{ -1,0 }, north{ 0,1 }, south{ 0,-1 };
	Tile2d position = player_tile_ + Tile2d{ -level_of_adjacency, -level_of_adjacency };

	for (auto dir : { east, north, west, south })
	{
			for (int i=-level_of_adjacency; i<=level_of_adjacency; i++)
			{
				position += dir;
				if (position.x >= 0 && position.x < procedural_parameters_->rows &&
					position.y >= 0 && position.y < procedural_parameters_->columns)
					adjacent_tiles.insert(int{ position.x + procedural_parameters_->rows * position.y });
			}
	}

}



///////// Act //////////////
void AGeneratorBase::loadTilesAsync(const TileList& tiles_to_load)
{

}

void AGeneratorBase::renderTilesAsync(const TileList& tiles_to_render)
{
	for (auto tile : tiles_to_render)
	{
		UE_LOG(LogTemp, Log, TEXT("loading tile %d"), tile);
		if (procedural_mesh_[tile].vertex_list.Num() > 1)
		{
			procedural_mesh_component_->SetMaterial(tile, material_);

			procedural_mesh_component_->CreateMeshSection_LinearColor(tile,
				procedural_mesh_[tile].vertex_list,
				procedural_mesh_[tile].triangle_list,
				TArray < FVector >(),
				TArray < FVector2D >(),
				procedural_mesh_[tile].color_list,
				TArray < FProcMeshTangent >(),
				true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile in section %d was not loaded into memory"), tile);
		}
	}
}

void AGeneratorBase::derenderTilesAsync(const TileList& tiles_to_derender)
{
	for (auto tile : tiles_to_derender)
		procedural_mesh_component_->ClearMeshSection(tile);
}

void AGeneratorBase::unloadTilesAsync(const TileList& tiles_to_unload)
{
	for (auto tile : tiles_to_unload)
		procedural_mesh_[tile].clear();
}






void AGeneratorBase::inverseIntersection(const TileList& a, const TileList& b, TileList& unique_to_a, TileList& unique_to_b)
{
	std::set_difference(a.begin(), a.end(),
		b.begin(), b.end(),
		std::inserter(unique_to_a, unique_to_a.begin()));
	std::set_difference(b.begin(), b.end(),
		a.begin(), a.end(),
		std::inserter(unique_to_b, unique_to_b.begin()));
};