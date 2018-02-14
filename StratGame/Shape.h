#pragma once

#include "Includes.h"
#include "VertexBufferController.h"
#include "Vertex.h"

class Shape {
public:
	~Shape();
	virtual int AddSelfForRendering(VertexBufferController* vbc, bool dynamic) = 0;
	void Translate(float x, float y, float z);
	void Scale(float scaleFactor);
protected:
	struct VertexIndicesForFace { int first, second, third, fourth; };
	void AddSquareFace(VertexIndicesForFace vertexIndices, D3DXVECTOR3 normal);
	void AddSquareFace(VertexIndicesForFace vertexIndices);
	void AddTriangleFace(VertexIndicesForFace vertexIndices, D3DXVECTOR3 normal);
	void AddTriangleFace(VertexIndicesForFace vertexIndices);
	D3DXVECTOR3 CalculateNormal(Vertex* primitive);

	VertexBufferController* vbc;
	Vertex* vertices;
	int numVertices;
};