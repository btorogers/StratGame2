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
	SquarePyramid spChevron(0.0f, 0.0f, 0.0f, 0.25f, 0.4f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), vbc);
	modelIDIndices.push_back(spChevron.AddSelfForRendering(true));
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

	// tree
	Sphere sphereTree(0.0f, 0.75f, 0.0f, 0.5f, D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f), vbc);
	Cuboid cuboidTree(0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, D3DXCOLOR(0.75f, 0.5f, 0.0f, 1.0f), vbc);

	modelIDIndices.push_back(sphereTree.AddSelfForRendering(true));
	cuboidTree.AddSelfForRendering(true);
	modelSizes.push_back(996);

	// rock
	Cuboid cuboidRock(0.0f, -0.4f, 0.2f, 0.4f, 0.6f, 0.4f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f), vbc);
	Cuboid cuboidRock2(-0.25f, -0.5f, 0.0f, 0.2f, 0.3f, 0.2f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f), vbc);
	Cuboid cuboidRock3(0.25f, -0.5f, 0.0f, 0.5f, 0.15f, 0.5f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f), vbc);
	modelIDIndices.push_back(cuboidRock.AddSelfForRendering(true));
	cuboidRock2.AddSelfForRendering(true);
	cuboidRock3.AddSelfForRendering(true);
	modelSizes.push_back(108);

	// gatherer
	Cuboid cuboidGatherer(0.0f, 0.0f, -0.05f, 0.5f, 0.7f, 0.05f, D3DXCOLOR(0.2f, 0.6f, 0.2f, 1.0f), vbc);
	Cuboid cuboidGatherer2(0.0f, 0.0f, 0.0f, 0.1f, 1.0f, 0.1f, D3DXCOLOR(0.4f, 0.2f, 0.0f, 1.0f), vbc);
	Cuboid cuboidGatherer3(0.0f, 0.4f, -0.05f, 0.55f, 0.1f, 0.1f, D3DXCOLOR(0.4f, 0.2f, 0.0f, 1.0f), vbc);
	modelIDIndices.push_back(cuboidGatherer.AddSelfForRendering(true));
	cuboidGatherer2.AddSelfForRendering(true);
	cuboidGatherer3.AddSelfForRendering(true);
	modelSizes.push_back(108);

	// stockpile
	Cuboid cuboidStockpile(0.0f, -0.5f, 0.0f, 2.99f, 0.05f, 2.99f, COLOR(200, 150, 50), vbc);
	modelIDIndices.push_back(cuboidStockpile.AddSelfForRendering(true));
	modelSizes.push_back(36);

}


