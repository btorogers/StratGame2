#include "SquarePyramid.h"

// 1-4-3
// |/ \|
// 0 - 2

SquarePyramid::SquarePyramid(float x, float y, float z, float baseRadius, float height, D3DXCOLOR color) {
	numVertices = 5;
	float hr = baseRadius / 2.0f;

	vertices = new Vertex[numVertices];
	vertices[0] = { D3DXVECTOR3(-hr, 0, -hr), color };
	vertices[1] = { D3DXVECTOR3(-hr, 0, hr), color };
	vertices[2] = { D3DXVECTOR3(hr, 0, -hr), color };
	vertices[3] = { D3DXVECTOR3(hr, 0, hr), color };
	vertices[4] = { D3DXVECTOR3(0, height, 0), color };

	Translate(x, y, z);
}

int SquarePyramid::AddSelfForRendering(VertexBufferController* vbc, bool dynamic) {
	this->vbc = vbc;
	int ret = vbc->lock(16, 18, false);

	// front
	AddTriangleFace({ 0,4,2 });
	// right
	AddTriangleFace({ 2,4,3 });
	// back
	AddTriangleFace({ 3,4,1 });
	//left
	AddTriangleFace({ 1,4,0 });
	// bottom
	AddSquareFace({ 0,2,1,3 });

	vbc->commit();
	return ret;
}