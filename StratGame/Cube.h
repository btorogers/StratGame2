#pragma once

#include "Vertex.h"
#include "VertexBufferController.h"

class Cube {
public:
	Cube(float x, float y, float z, float edgeLength, D3DXCOLOR color);
	Vertex* GetFullCoverStrip(unsigned int *retLength);
	void AddSelfForRendering(VertexBufferController* vbc);
private:
	struct IndexesForFace { unsigned int first, second, third, fourth; };
	void AddFace(VertexBufferController* vbc, IndexesForFace indexes, D3DXVECTOR3 direction);
	Vertex* vertices;
};