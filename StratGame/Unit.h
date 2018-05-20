#pragma once

#include "GameObject.h"

class Unit : public GameObject {
public:
	Unit(GameController* game, int x, int y);

	virtual void Render() = 0;
	virtual void Update();
	virtual void ReceiveClick(int x, int y);

protected:
	void GeneratePath();

	void FindOpenNearTarget();

	struct PathCoordinates {
		int x;
		int y;
	};

	std::stack<PathCoordinates> path;
	int moveSpeed = 25;
	int updatesSinceMove = 0;
	int xTarget, yTarget;
};