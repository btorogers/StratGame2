#pragma once

#include "GameObject.h"
#include "GameController.h"

class Stockpile : public GameObject {
public:
	Stockpile(GameController* game, int x, int y);

	void Render();
	void Update();

	void GiveWood();
	void GiveStone();

private:
	int storedWood, storedStone;
};