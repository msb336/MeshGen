#pragma once
#include "CoreMinimal.h"
#include <array>

using Vertices = TArray<FVector>;
using Triangles = TArray<int32>;
using UV = TArray<FVector2D>;

struct TriangularMesh {
	Vertices vertices;
	Triangles triangles;
	UV uvs;
};