#pragma once

#include "GameObject.h"
#include "GameController.h"

class Rock : public GameObject {
public:
	Rock(GameController* game, int x, int y);

	void Render();
	void Update();
};