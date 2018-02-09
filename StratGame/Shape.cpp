#include "Shape.h"

Shape::~Shape() {
	delete[] vertices;
}

void Shape::AddSquareFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices, D3DXVECTOR3 normal) {
	Vertex* v = new Vertex[4]{
		vertices[vertexIndices.first], vertices[vertexIndices.second], vertices[vertexIndices.third], vertices[vertexIndices.fourth]
	};
	v[0].normal = v[1].normal = v[2].normal = v[3].normal = normal;
	int* i2 = new int[6]{ 0,1,2,2,1,3 };
	vbc->AppendVertices(v, i2, 4, 6);
	delete[] i2;
	delete[] v;
}

void Shape::AddSquareFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices) {
	Vertex* v = new Vertex[4]{
		vertices[vertexIndices.first], vertices[vertexIndices.second], vertices[vertexIndices.third], vertices[vertexIndices.fourth]
	};
	v[0].normal = v[1].normal = v[2].normal = v[3].normal = CalculateNormal(v);
	int* i2 = new int[6]{ 0,1,2,2,1,3 };
	vbc->AppendVertices(v, i2, 4, 6);
	delete[] i2;
	delete[] v;
}

void Shape::AddTriangleFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices, D3DXVECTOR3 normal) {
	Vertex* v = new Vertex[3]{
		vertices[vertexIndices.first], vertices[vertexIndices.second], vertices[vertexIndices.third]
	};
	v[0].normal = v[1].normal = v[2].normal = normal;
	vbc->AppendPrimitive(v);
	delete[] v;
}

void Shape::AddTriangleFace(VertexBufferController* vbc, VertexIndicesForFace vertexIndices) {
	Vertex* v = new Vertex[3]{
		vertices[vertexIndices.first], vertices[vertexIndices.second], vertices[vertexIndices.third]
	};
	v[0].normal = v[1].normal = v[2].normal = CalculateNormal(v);
	vbc->AppendPrimitive(v);
	delete[] v;
}

D3DXVECTOR3 Shape::CalculateNormal(Vertex* v)
{
	D3DXVECTOR3 vec1 = D3DXVECTOR3(v[1].position - v[0].position);
	D3DXVECTOR3 vec2 = D3DXVECTOR3(v[2].position - v[0].position);
	D3DXVECTOR3 normal = D3DXVECTOR3();
	D3DXVec3Cross(&normal, &vec1, &vec2);
	return normal;
}
