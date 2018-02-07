#pragma once

#include "Includes.h"
#include "Shape.h"

class SquarePyramid : public Shape {
public:
	SquarePyramid(float x, float y, float z, float baseRadius, float height, D3DXCOLOR color);
	void AddSelfForRendering(VertexBufferController* vbc);
protected:
	void AddFace(unsigned int vertCount, VertexBufferController* vbc, IndexesForFace indexes, D3DXVECTOR3 direction);
};