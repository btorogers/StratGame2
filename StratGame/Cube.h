#pragma once

#include "Vertex.h"

class Cube {
public:
	Cube(float x, float y, float z, float edgeLength, D3DXCOLOR color);
	Vertex *GetVertices();
	Vertex *GetFullCoverStrip(unsigned int *retLength);
private:
	Vertex *vertices;
};