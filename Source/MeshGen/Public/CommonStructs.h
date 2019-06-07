#pragma once
#include "CoreMinimal.h"
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <cmath>

#include "ProceduralMeshComponent.h"
struct ProceduralParameters {
	float scale_x;
	float scale_y;
	int rows;
	int columns;
	int tile_size;
	int load_adjacency = 2;
	int render_adjacency = 1;
};
struct Position {
	Position(float xx, float yy, float zz) : x{ xx }, y{ yy }, z{ zz } {}
	Position() : x{ 0 }, y{ 0 }, z{ 0 }{}
	float x;
	float y;
	float z;
};

struct MeshSection {
	TArray<FVector> vertex_list;
	TArray<int32> triangle_list;
	TArray < FVector > normal_list;
	TArray < FVector2D > uv;
	TArray<FLinearColor> color_list;
	TArray < FProcMeshTangent > tangents;
	inline void clear() 
	{ 
		vertex_list.Empty(); triangle_list.Empty();
		normal_list.Empty(); uv.Empty();
		color_list.Empty(); tangents.Empty();
	}
};

struct Tile2d {
	int x;
	int y;
	Tile2d() : x(0), y(0) {};
	Tile2d(int xx, int yy) : x(xx), y(yy) {};
	inline Tile2d operator+(Tile2d t2) { return Tile2d{ x + t2.x, y + t2.y }; };
	inline Tile2d operator*(float mult) { return Tile2d{ int(x*mult), int(y*mult) }; };
	inline void operator+=(Tile2d t2) { x += t2.x; y += t2.y; };
};


typedef std::vector<MeshSection> ProceduralMesh;
typedef std::set<int> TileList;