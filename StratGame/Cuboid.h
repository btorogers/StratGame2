#pragma once

#include "Includes.h"
#include "Shape.h"

class Cuboid: public Shape {
public:
	Cuboid(float x, float y, float z, float radius, D3DXCOLOR color, VertexBufferController* vbc);
	Cuboid(float x, float y, float z, float xRadius, float yRadius, float zRadius, D3DXCOLOR color, VertexBufferController* vbc);
	int AddSelfForRendering(bool dynamic);
};