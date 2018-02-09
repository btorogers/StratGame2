#pragma once

#include "Includes.h"
#include "VertexBufferController.h"
#include "Vertex.h"

class Shape {
public:
	~Shape();
	virtual void AddSelfForRendering(VertexBufferController* vbc) = 0;
protected:
	struct VertexIndicesForFace { unsigned int first, second, third, fourth; };
	void AddSquareFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices, D3DXVECTOR3 normal);
	void AddSquareFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices);
	void AddTriangleFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices, D3DXVECTOR3 normal);
	void AddTriangleFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices);
	D3DXVECTOR3 CalculateNormal(Vertex* primitive);
	Vertex* vertices;
};