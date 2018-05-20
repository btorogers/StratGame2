#include "WorldController.h"
#include "GameObject.h"
#include "Gatherer.h"
#include "Rock.h"
#include "Tree.h"
#include "Stockpile.h"

WorldController::WorldController(GameController* game): game(game) {

}

WorldController::~WorldController() {
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete *it;
	}
}

void WorldController::UpdateAll() {
	objectsLock.lock();
	for (GameObject* o : objects) {
		o->Update();
	}
	objectsLock.unlock();
}

void WorldController::RenderAll() {
	objectsLock.lock();
	for (GameObject* o : objects) {
		o->Render();
	}
	objectsLock.unlock();
}

std::vector<GameObject*>* WorldController::GetObjects() {
	return &objects;
}

std::mutex * WorldController::GetObjectsLock() {
	return &objectsLock;
}

void WorldController::Generate() {
	objectsLock.lock();

	objects.push_back(new Stockpile(game, RANDINT(GRID_X - 2) + 1, RANDINT(GRID_Y - 2) + 1));

	for (int i = 0; i < GATHERER_COUNT; i++) {
		int gridX = RANDINT(GRID_X);
		int gridY = RANDINT(GRID_Y);
		while (grid[gridX][gridY]) {
			gridX = RANDINT(GRID_X);
			gridY = RANDINT(GRID_Y);
		}
		objects.push_back(new Gatherer(game, gridX, gridY));
	}

	for (int i = 0; i < TREE_COUNT; i++) {
		int gridX = RANDINT(GRID_X);
		int gridY = RANDINT(GRID_Y);
		while (grid[gridX][gridY]) {
			gridX = RANDINT(GRID_X);
			gridY = RANDINT(GRID_Y);
		}
		objects.push_back(new Tree(game, gridX, gridY));
	}

	for (int i = 0; i < ROCK_COUNT; i++) {
		int gridX = RANDINT(GRID_X);
		int gridY = RANDINT(GRID_Y);
		while (grid[gridX][gridY]) {
			gridX = RANDINT(GRID_X);
			gridY = RANDINT(GRID_Y);
		}
		objects.push_back(new Rock(game, gridX, gridY));
	}

	objectsLock.unlock();
}
