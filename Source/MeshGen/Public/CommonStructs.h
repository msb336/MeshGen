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

struct MeshSection {
	TArray<FVector> vertex_list;
	TArray<int32> triangle_list;
	TArray < FVector > normal_list;
	TArray < FVector2D > uv;
	TArray<FLinearColor> color_list;
	TArray < FProcMeshTangent > tangents;
};

typedef std::vector<MeshSection> ProceduralMesh;