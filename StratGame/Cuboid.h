#pragma once

#include "Includes.h"
#include "Shape.h"

class Cuboid: public Shape {
public:
	Cuboid(float x, float y, float z, float radius, D3DXCOLOR color);
	Cuboid(float x, float y, float z, float xRadius, float yRadius, float zRadius, D3DXCOLOR color);
	Vertex* GetFullCoverStrip(unsigned int *retLength);
	void AddSelfForRendering(VertexBufferController* vbc);
};