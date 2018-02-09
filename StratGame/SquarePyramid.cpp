#include "SquarePyramid.h"

// 1-4-3
// |/ \|
// 0 - 2

SquarePyramid::SquarePyramid(float x, float y, float z, float baseRadius, float height, D3DXCOLOR color) {
	float halfBaseRadius = baseRadius / 2.0f;
	x -= halfBaseRadius;
	y -= halfBaseRadius;
	z -= halfBaseRadius;

	vertices = new Vertex[5];
	vertices[0] = { D3DXVECTOR3(x, y, z), color };
	vertices[1] = { D3DXVECTOR3(x, y, z + baseRadius), color };
	vertices[2] = { D3DXVECTOR3(x + baseRadius, y, z), color };
	vertices[3] = { D3DXVECTOR3(x + baseRadius, y, z + baseRadius), color };
	vertices[4] = { D3DXVECTOR3(x + halfBaseRadius, y + height, z + halfBaseRadius), color };
}

void SquarePyramid::AddSelfForRendering(VertexBufferController* vbc) {
	vbc->lock(24, 10000, VertexBufferController::WAIT_UNTIL_FREE);

	// front
	AddTriangleFace(vbc, { 0,4,2 });
	// right
	AddTriangleFace(vbc, { 2,4,3 });
	// back
	AddTriangleFace(vbc, { 3,4,1 });
	//left
	AddTriangleFace(vbc, { 1,4,0 });
	// bottom
	AddSquareFace(vbc, { 0,2,1,3 });

	vbc->commit();
}