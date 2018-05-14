#include "Cuboid.h"

//   4 - 5
//  /   /|
// 0 - 1 7
// |   |/
// 2 - 3


Cuboid::Cuboid(float x, float y, float z, float radius, D3DXCOLOR color, VertexBufferController* vbc) {
	this->vbc = vbc;
	numVertices = 8;
	float hr = radius / 2.0f;

	vertices = new Vertex[numVertices];
	vertices[0] = { D3DXVECTOR3(-hr, hr, -hr), color };
	vertices[1] = { D3DXVECTOR3(hr, hr, -hr), color };
	vertices[2] = { D3DXVECTOR3(-hr, -hr, -hr), color };
	vertices[3] = { D3DXVECTOR3(hr, -hr, -hr), color };
	vertices[4] = { D3DXVECTOR3(-hr, hr, hr), color };
	vertices[5] = { D3DXVECTOR3(hr, hr, hr), color };
	vertices[6] = { D3DXVECTOR3(-hr, -hr, hr), color };
	vertices[7] = { D3DXVECTOR3(hr, -hr, hr), color };

	Translate(x, y, z);
}

Cuboid::Cuboid(float x, float y, float z, float xRadius, float yRadius, float zRadius, D3DXCOLOR color, VertexBufferController* vbc) {
	this->vbc = vbc;
	numVertices = 8;
	float hxr = xRadius / 2.0f;
	float hyr = yRadius / 2.0f;
	float hzr = zRadius / 2.0f;

	vertices = new Vertex[numVertices];
	vertices[0] = { D3DXVECTOR3(-hxr, hyr, -hzr), color };
	vertices[1] = { D3DXVECTOR3(hxr, hyr, -hzr), color };
	vertices[2] = { D3DXVECTOR3(-hxr, -hyr, -hzr), color };
	vertices[3] = { D3DXVECTOR3(hxr, -hyr, -hzr), color };
	vertices[4] = { D3DXVECTOR3(-hxr, hyr, hzr), color };
	vertices[5] = { D3DXVECTOR3(hxr, hyr, hzr), color };
	vertices[6] = { D3DXVECTOR3(-hxr, -hyr, hzr), color };
	vertices[7] = { D3DXVECTOR3(hxr, -hyr, hzr), color };

	Translate(x, y, z);
}

int Cuboid::AddSelfForRendering(bool dynamic) {
	int ret = vbc->lock(24, 36, dynamic);

	//front
	AddSquareFace({ 0,1,2,3 });
	//top
	AddSquareFace({ 0,4,1,5 });
	//back
	AddSquareFace({ 5,4,7,6 });
	//bottom
	AddSquareFace({ 2,3,6,7 });
	//left
	AddSquareFace({ 4,0,6,2 });
	//right
	AddSquareFace({ 1,5,3,7 });
	vbc->commit();

	return ret;
}
