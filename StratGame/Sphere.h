#pragma once

#include "Includes.h"
#include "Shape.h"

class Sphere: public Shape {
public:
	// do NOT put this higher than 5
	static const int precision = 3;

	Sphere(float x, float y, float z, float radius, D3DXCOLOR color, VertexBufferController* vbc);
	int AddSelfForRendering(bool dynamic);
private:
	int GetMiddle(int firstIndex, int secondIndex);

	std::vector<Vertex> vecVertices;
	std::vector<VertexIndicesForFace> faces;
};