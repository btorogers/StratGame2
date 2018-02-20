#pragma once
#include "GameObject.h"
class Tree : public GameObject {
public:
	Tree(VertexBufferController* vbc, int x, int y);
	~Tree();

	void Render();
	void Update();

private:
	static int indexOfModel;
	static bool modelLoaded;
	int* instanceIndex;
};

