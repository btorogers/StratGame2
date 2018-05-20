#pragma once

#include "Unit.h"

class Gatherer : public Unit {
public:
	Gatherer(GameController* game, int x, int y);
	~Gatherer();

	void Render();
	void Update();
	void ReceiveClick(int x, int y);
};