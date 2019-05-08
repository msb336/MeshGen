#pragma once
#include "CoreMinimal.h"
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

struct MeshParameters {
	float scale_x;
	float scale_y;
	int rows;
	int columns;
	int tile_size;
};
struct Position {
	Position ( float xx, float yy, float zz) : 
		x{ xx }, y{ yy }, z{ zz } {}
	float x;
	float y;
	float z;
};

using TileArray = std::array<int, 2>;
using Tile = int;
using TileSet = std::set<Tile>;
//using Vertices = TArray<FVector>;
//using Triangles = TArray<int32>;
//using UV = TArray<FVector2D>;
//
//struct TriangularMesh {
//	Vertices vertices;
//	Triangles triangles;
//	UV uvs;
//};
//
//struct SectionBoundary {
//	std::array<float, 2> lower;
//	std::array<float, 2> upper;
//};