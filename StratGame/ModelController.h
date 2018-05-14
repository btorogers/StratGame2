#pragma once

#include "Includes.h"
#include "VertexBufferController.h"
#include "SquarePyramid.h"

enum ModelID {
	SELECTED_CHEVRON = 0,
	SELECTION_TILE = 1
};

class ModelController {
public:
	ModelController(VertexBufferController* vbc);

	int GetIndexOfModel(ModelID modelID);
	int GetModelSize(ModelID modelID);

private:
	void LoadModels();

	VertexBufferController* vbc;
	std::vector<int> modelIDIndices;
	std::vector<int> modelSizes;
};