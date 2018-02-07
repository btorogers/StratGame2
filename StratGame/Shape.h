#pragma once

#include "Includes.h"
#include "VertexBufferController.h"
#include "Vertex.h"

class Shape {
public:
	~Shape() { delete[] vertices; };
	virtual void AddSelfForRendering(VertexBufferController* vbc) = 0;
protected:
	struct IndexesForFace { unsigned int first, second, third, fourth; };
	virtual void AddFace(VertexBufferController* vbc, IndexesForFace indexes, D3DXVECTOR3 direction) {};
	Vertex* vertices;
};