#include "Cube.h"

//   4 - 5
//  /   /|
// 0 - 1 7
// |   |/
// 2 - 3
Cube::Cube(float x, float y, float z, float edgeLength, D3DXCOLOR color) {
	vertices = new Vertex[8];
	vertices[0] = { x - (edgeLength / 2.0f), y + (edgeLength / 2.0f), z - (edgeLength / 2.0f), color };
	vertices[1] = { x + (edgeLength / 2.0f), y + (edgeLength / 2.0f), z - (edgeLength / 2.0f), color };
	vertices[2] = { x - (edgeLength / 2.0f), y - (edgeLength / 2.0f), z - (edgeLength / 2.0f), color };
	vertices[3] = { x + (edgeLength / 2.0f), y - (edgeLength / 2.0f), z - (edgeLength / 2.0f), color };
	vertices[4] = { x - (edgeLength / 2.0f), y + (edgeLength / 2.0f), z + (edgeLength / 2.0f), color };
	vertices[5] = { x + (edgeLength / 2.0f), y + (edgeLength / 2.0f), z + (edgeLength / 2.0f), color };
	vertices[6] = { x - (edgeLength / 2.0f), y - (edgeLength / 2.0f), z + (edgeLength / 2.0f), color };
	vertices[7] = { x + (edgeLength / 2.0f), y - (edgeLength / 2.0f), z + (edgeLength / 2.0f), color };
}

Vertex* Cube::GetFullCoverStrip(unsigned int *retLength) {
	*retLength = 14;
	return new Vertex[14]{
		vertices[2],
		vertices[0],
		vertices[3],
		vertices[1],
		vertices[5],
		vertices[0],
		vertices[4],
		vertices[2],
		vertices[6],
		vertices[3],
		vertices[7],
		vertices[5],
		vertices[6],
		vertices[4]
	};
}

void Cube::AddSelfForRendering(VertexBufferController* vbc) {
	//front
	IndexesForFace indexes = {0,1,2,3};
	AddFace(vbc, indexes, D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	//top
	indexes = {0,4,1,5};
	AddFace(vbc, indexes, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//back
	indexes = {5,4,7,6};
	AddFace(vbc, indexes, D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	//bottom
	indexes = {2,3,6,7};
	AddFace(vbc, indexes, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	//left
	indexes = {4,0,6,2};
	AddFace(vbc, indexes, D3DXVECTOR3(-1.0f, 0.0f, 0.0f));
	//right
	indexes = {1,5,3,7};
	AddFace(vbc, indexes, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
}

void Cube::AddFace(VertexBufferController* vbc, IndexesForFace indexes, D3DXVECTOR3 direction) {
	Vertex* v = new Vertex[4]{
		vertices[indexes.first], vertices[indexes.second], vertices[indexes.third], vertices[indexes.fourth]
	};
	v[0].normal = v[1].normal = v[2].normal = v[3].normal = direction;
	vbc->AppendVertices(v, 4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
