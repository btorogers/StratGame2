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
	vertices[0] = { D3DXVECTOR3(x, y + radius, z), color };
	vertices[1] = { D3DXVECTOR3(x + radius, y + radius, z), color };
	vertices[2] = { D3DXVECTOR3(x, y, z), color };
	vertices[3] = { D3DXVECTOR3(x + radius, y, z), color };
	vertices[4] = { D3DXVECTOR3(x, y + radius, z + radius), color };
	vertices[5] = { D3DXVECTOR3(x + radius, y + radius, z + radius), color };
	vertices[6] = { D3DXVECTOR3(x, y, z + radius), color };
	vertices[7] = { D3DXVECTOR3(x + radius, y, z + radius), color };
}

Cuboid::Cuboid(float x, float y, float z, float xr, float yr, float zr, D3DXCOLOR color) {
	x -= xr / 2.0f;
	y -= yr / 2.0f;
	z -= zr / 2.0f;

	vertices = new Vertex[8];
	vertices[0] = { D3DXVECTOR3(x, y + yr, z), color };
	vertices[1] = { D3DXVECTOR3(x + xr, y + yr, z), color };
	vertices[2] = { D3DXVECTOR3(x, y, z), color };
	vertices[3] = { D3DXVECTOR3(x + xr, y, z), color };
	vertices[4] = { D3DXVECTOR3(x, y + yr, z + zr), color };
	vertices[5] = { D3DXVECTOR3(x + xr, y + yr, z + zr), color };
	vertices[6] = { D3DXVECTOR3(x, y, z + zr), color };
	vertices[7] = { D3DXVECTOR3(x + xr, y, z + zr), color };
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
	vbc->lock(24, 36, VertexBufferController::WAIT_UNTIL_FREE);

	//front
	AddSquareFace(vbc, { 0,1,2,3 });
	//top
	AddSquareFace(vbc, { 0,4,1,5 });
	//back
	AddSquareFace(vbc, { 5,4,7,6 });
	//bottom
	AddSquareFace(vbc, { 2,3,6,7 });
	//left
	AddSquareFace(vbc, { 4,0,6,2 });
	//right
	AddSquareFace(vbc, { 1,5,3,7 });

	vbc->commit();
}
