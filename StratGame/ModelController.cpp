#include "ModelController.h"

ModelController::ModelController(VertexBufferController* vbc) {
	this->vbc = vbc;

	LoadModels();
}

int ModelController::GetIndexOfModel(ModelID modelID) {
	return modelIDIndices[modelID];
}

int ModelController::GetModelSize(ModelID modelID) {
	return modelSizes[modelID];
}

void ModelController::LoadModels() {
	// selection chevron
	SquarePyramid s(0.0f, 0.0f, 0.0f, 0.25f, 0.4f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), vbc);
	modelIDIndices.push_back(s.AddSelfForRendering(true));
	modelSizes.push_back(18);

	// selection tile
	modelIDIndices.push_back(vbc->lock(8, 12, true));
	modelSizes.push_back(12);

	D3DXCOLOR colorEdge(1.0f, 1.0f, 0.5f, 1.0f);
	D3DXCOLOR colorCenter(0.7f, 1.0f, 0.7f, 1.0f);

	Vertex* vertices = new Vertex[8]{
		{ D3DXVECTOR3(-0.5f, -0.001f, 0.5f), colorEdge, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(0.5f, -0.001f, 0.5f), colorEdge, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(-0.5f, -0.001f, -0.5f), colorEdge, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(0.5f, -0.001f, -0.5f), colorEdge, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(-0.45f, 0.001f, 0.45f), colorCenter, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(0.45f, 0.001f, 0.45f), colorCenter, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(-0.45f, 0.001f, -0.45f), colorCenter, D3DXVECTOR3(0.0f, 1.0f, 0.0f) },
		{ D3DXVECTOR3(0.45f, 0.001f, -0.45f), colorCenter, D3DXVECTOR3(0.0f, 1.0f, 0.0f) }
	};
	int* indices = new int[12]{ 0,1,2,2,1,3,4,5,6,6,5,7 };
	vbc->AppendVertices(vertices, indices, 8, 12);
	vbc->commit();
	delete[] indices;
	delete[] vertices;
}


