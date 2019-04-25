#pragma once
#include "CommonStructs.h"
class MeshGenerator
{
	private:
		MeshObject mesh_;
		PointCloud points_;
		VertexIndex vertices_;
		//PropertyList properties_;
		float min_draw_distance_;


	virtual void generateMesh(const Dimensions&, const int&) = 0;
	virtual void extend(const Point2d&) = 0;
	MeshObject update();
};