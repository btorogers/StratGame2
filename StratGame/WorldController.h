#pragma once

#include "Includes.h"

#define TREE_COUNT 5
#define ROCK_COUNT 5
#define STOCKPILE_COUNT 1
#define GATHERER_COUNT 2

class GameObject;
class GameController;

class WorldController {
public:
	WorldController(GameController* game);
	~WorldController();

	void UpdateAll();
	void RenderAll();

	void Generate();

	std::vector<GameObject*>* GetObjects();
	std::mutex* GetObjectsLock();

	GameObject* grid[GRID_X][GRID_Y];
private:
	GameController* game;

	std::vector<GameObject*> objects;
	std::mutex objectsLock;
};