#include "Sphere.h"

// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

Sphere::Sphere(float x, float y, float z, float radius, D3DXCOLOR color) {
	// simple trigonometry *laughs in scatter arrow*
	//  /|
	//r/ |t
	// --|
	//  u
	static float goldenRatio = (1.0f + sqrt(5.0f)) / 2.0f;
	static float r = sqrt(1 + pow((1.0f + sqrt(5.0f)) / 2.0f, 2));
	static float t = goldenRatio / r;
	static float u = 1 / r;
	
	//vertices = new Vertex[numVertices];
	vecVertices.push_back({ D3DXVECTOR3(-u, t, 0), color });
	vecVertices.push_back({ D3DXVECTOR3(u, t, 0), color });
	vecVertices.push_back({ D3DXVECTOR3(-u, -t, 0), color });
	vecVertices.push_back({ D3DXVECTOR3(u, -t, 0), color });

	vecVertices.push_back({ D3DXVECTOR3(0, -u, t), color });
	vecVertices.push_back({ D3DXVECTOR3(0, u, t), color });
	vecVertices.push_back({ D3DXVECTOR3(0, -u, -t), color });
	vecVertices.push_back({ D3DXVECTOR3(0, u, -t), color });

	vecVertices.push_back({ D3DXVECTOR3(t, 0, -u), color });
	vecVertices.push_back({ D3DXVECTOR3(t, 0, u), color });
	vecVertices.push_back({ D3DXVECTOR3(-t, 0, -u), color });
	vecVertices.push_back({ D3DXVECTOR3(-t, 0, u), color });

	faces.push_back({ 0, 11, 5 });
	faces.push_back({ 0, 5, 1 });
	faces.push_back({ 0, 1, 7 });
	faces.push_back({ 0, 7, 10 });
	faces.push_back({ 0, 10, 11 });
	faces.push_back({ 1, 5, 9 });
	faces.push_back({ 5, 11, 4 });
	faces.push_back({ 11, 10, 2 });
	faces.push_back({ 10, 7, 6 });
	faces.push_back({ 7, 1, 8 });
	faces.push_back({ 3, 9, 4 });
	faces.push_back({ 3, 4, 2 });
	faces.push_back({ 3, 2, 6 });
	faces.push_back({ 3, 6, 8 });
	faces.push_back({ 3, 8, 9 });
	faces.push_back({ 4, 9, 5 });
	faces.push_back({ 2, 4, 11 });
	faces.push_back({ 6, 2, 10 });
	faces.push_back({ 8, 6, 7 });
	faces.push_back({ 9, 8, 1 });

	
	for (int recursionLevel = 1; recursionLevel < Sphere::precision; recursionLevel++) {
		std::vector<VertexIndicesForFace> newFaces;
		for (VertexIndicesForFace i : faces) {
			int a = GetMiddle(i.first, i.second);
			int b = GetMiddle(i.second, i.third);
			int c = GetMiddle(i.third, i.first);

			newFaces.push_back({ i.first, a, c });
			newFaces.push_back({ i.second, b, a });
			newFaces.push_back({ i.third, c, b });
			newFaces.push_back({ a, b, c });
		}

		faces.clear();
		faces = newFaces;
	}

	for (Vertex &v : vecVertices) {
		D3DXVec3Normalize(&v.normal, &v.position);
	}

	numVertices = vecVertices.size();
	this->vertices = new Vertex[numVertices];
	memcpy(vertices, &vecVertices[0], sizeof(Vertex) * numVertices);

	vecVertices.clear();

	Scale(radius);
	Translate(x, y, z);
}

int Sphere::AddSelfForRendering(VertexBufferController* vbc, bool dynamic) {
	this->vbc = vbc;
	int ret = vbc->lock(numVertices, faces.size() * 3, dynamic);

	std::vector<int> vecIndices;
	for (VertexIndicesForFace i : faces) {
		vecIndices.push_back(i.first);
		vecIndices.push_back(i.second);
		vecIndices.push_back(i.third);
	}

	int* indices = new int[vecIndices.size()];
	memcpy(indices, &vecIndices[0], sizeof(int) * vecIndices.size());

	vbc->AppendVertices(vertices, indices, numVertices, faces.size() * 3);
	vbc->commit();

	vecIndices.clear();
	delete[] indices;

	return ret;
}

int Sphere::GetMiddle(int firstIndex, int secondIndex) {
	D3DXVECTOR3 vectorNew = (vecVertices.at(firstIndex).position + vecVertices.at(secondIndex).position) / 2.0f;
	D3DXVec3Normalize(&vectorNew, &vectorNew);
	Vertex vertexNew = { vectorNew, vecVertices.at(firstIndex).color };

	for (int x = 0; x < (int)vecVertices.size(); x++) {
		Vertex compV = vecVertices.at(x);
		if (compV.position == vertexNew.position) {
			return x;
		}
	}

	vecVertices.push_back(vertexNew);
	return vecVertices.size() - 1;
}