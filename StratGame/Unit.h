#pragma once

#include "GameObject.h"
#include "GameController.h"

class Unit : public GameObject {
public:
	Unit(GameController* game, int x, int y);
	~Unit();

	void Render();
	void Update();
	void ReceiveClick(int x, int y);

private:
	void GeneratePath();

	struct PathCoordinates {
		int x;
		int y;
	};

	struct AStarNode {
		int gCost, hCost, fCost, x, y;
		AStarNode* parent;
	};

	GameController* game;
	std::stack<PathCoordinates> path;
	int moveSpeed = 25;
	int updatesSinceMove = 0;
	int xTarget, yTarget;

	static int indexOfModel;
	static bool modelLoaded;
	int* instanceIndex;

};