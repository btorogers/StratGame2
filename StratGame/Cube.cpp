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

Vertex *Cube::GetVertices() {
	return vertices;
}

Vertex *Cube::GetFullCoverStrip(unsigned int *retLength) {
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
