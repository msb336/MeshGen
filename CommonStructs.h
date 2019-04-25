#pragma once
#include <vector>

struct Point3d {
	float x;
	float y;
	float z;
};

struct Point2d {
	float x;
	float y;
};

struct Dimensions {
	float height;
	float width;
};

using PointCloud = std::vector<Point3d>;
using Vertex = int[3];
using VertexIndex = std::vector<Vertex>;

struct MeshObject {
	PointCloud points;
	VertexIndex vertices;
};