#pragma once

#include "GameObject.h"
#include "GameController.h"

class Tree : public GameObject {
public:
	Tree(GameController* game, int x, int y);

	void Render();
	void Update();
};

