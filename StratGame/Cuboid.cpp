#include "Cuboid.h"

//   4 - 5
//  /   /|
// 0 - 1 7
// |   |/
// 2 - 3


Cuboid::Cuboid(float x, float y, float z, float radius, D3DXCOLOR color) {
	x -= radius / 2.0f;
	y -= radius / 2.0f;
	z -= radius / 2.0f;
	vertices = new Vertex[8];
	vertices[0] = { x, y + radius, z, color };
	vertices[1] = { x + radius, y + radius, z, color };
	vertices[2] = { x, y, z, color };
	vertices[3] = { x + radius, y, z, color };
	vertices[4] = { x, y + radius, z + radius, color };
	vertices[5] = { x + radius, y + radius, z + radius, color };
	vertices[6] = { x, y, z + radius, color };
	vertices[7] = { x + radius, y, z + radius, color };
}

Cuboid::Cuboid(float x, float y, float z, float xr, float yr, float zr, D3DXCOLOR color) {
	x -= xr / 2.0f;
	y -= yr / 2.0f;
	z -= zr / 2.0f;
	vertices = new Vertex[8];
	vertices[0] = { x, y + yr, z, color };
	vertices[1] = { x + xr, y + yr, z, color };
	vertices[2] = { x, y, z, color };
	vertices[3] = { x + xr, y, z, color };
	vertices[4] = { x, y + yr, z + zr, color };
	vertices[5] = { x + xr, y + yr, z + zr, color };
	vertices[6] = { x, y, z + zr, color };
	vertices[7] = { x + xr, y, z + zr, color };
}

Vertex* Cuboid::GetFullCoverStrip(unsigned int *retLength) {
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

void Cuboid::AddSelfForRendering(VertexBufferController* vbc) {
	vbc->lock(24, VertexBufferController::WAIT_UNTIL_FREE);
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
	vbc->commit();
}

void Cuboid::AddFace(VertexBufferController* vbc, IndexesForFace indexes, D3DXVECTOR3 direction) {
	Vertex* v = new Vertex[4]{
		vertices[indexes.first], vertices[indexes.second], vertices[indexes.third], vertices[indexes.fourth]
	};
	v[0].normal = v[1].normal = v[2].normal = v[3].normal = direction;
	vbc->AppendVertices(v, 4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	delete[] v;
}
