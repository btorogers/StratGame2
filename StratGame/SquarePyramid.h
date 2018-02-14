#pragma once

#include "Includes.h"
#include "Shape.h"

class SquarePyramid : public Shape {
public:
	SquarePyramid(float x, float y, float z, float baseRadius, float height, D3DXCOLOR color);
	int AddSelfForRendering(VertexBufferController* vbc, bool dynamic);
};