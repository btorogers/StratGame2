#include "SquarePyramid.h"

// 1-4-3
// |/ \|
// 0 - 2

SquarePyramid::SquarePyramid(float x, float y, float z, float baseRadius, float height, D3DXCOLOR color) {
	x -= baseRadius / 2.0f;
	y -= height / 2.0f;
	z -= baseRadius / 2.0f;
	vertices = new Vertex[5];
	vertices[0] = { x, y, z, color };
	vertices[1] = { x, y, z + baseRadius, color };
	vertices[2] = { x + baseRadius, y, z, color };
	vertices[3] = { x + baseRadius, y, z + baseRadius, color };
	vertices[4] = { x + baseRadius / 2.0f, y + height, z + baseRadius / 2.0f, color };
}

void SquarePyramid::AddSelfForRendering(VertexBufferController* vbc) {
	vbc->lock(24, VertexBufferController::WAIT_UNTIL_FREE);
	// front
	IndexesForFace indexes = { 0,4,2 };
	AddFace(3, vbc, indexes, D3DXVECTOR3(0.0f, 1.0f, -1.0f));
	// right
	indexes = { 2,4,3 };
	AddFace(3, vbc, indexes, D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	// back
	indexes = { 3,4,1 };
	AddFace(3, vbc, indexes, D3DXVECTOR3(0.0f, 1.0f, 1.0f));
	//left
	indexes = { 1,4,0 };
	AddFace(3, vbc, indexes, D3DXVECTOR3(-1.0f, 1.0f, 0.0f));
	// bottom
	indexes = { 0,2,1,3 };
	AddFace(4, vbc, indexes, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	vbc->commit();
}

void SquarePyramid::AddFace(unsigned int vertCount, VertexBufferController* vbc, IndexesForFace indexes, D3DXVECTOR3 direction) {
	Vertex* v = new Vertex[4]{
		vertices[indexes.first], vertices[indexes.second], vertices[indexes.third], vertices[indexes.fourth]
	};
	v[0].normal = v[1].normal = v[2].normal = v[3].normal = direction;
	vbc->AppendVertices(v, vertCount, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	delete[] v;
}
