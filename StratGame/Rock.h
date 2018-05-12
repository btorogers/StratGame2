#pragma once

#include "GameObject.h"

class Rock : public GameObject {
public:
	Rock(VertexBufferController* vbc, int x, int y);
	~Rock();

	void Render();
	void Update();

private:
	static int indexOfModel;
	static bool modelLoaded;
	int* instanceIndex;
};