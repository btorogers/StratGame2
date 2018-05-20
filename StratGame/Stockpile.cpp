#include "Stockpile.h"

Stockpile::Stockpile(GameController * game, int x, int y) : GameObject(game, x, y) {
	for (int t = x - 1; t <= x + 1; t++) {
		for (int u = y - 1; u <= y + 1; u++) {
			world->grid[t][u] = this;
		}
	}
}

void Stockpile::Render() {
	if (instanceIndex)
		vbc->RenderInstanced(models->GetIndexOfModel(ModelID::STOCKPILE), models->GetModelSize(ModelID::STOCKPILE), *instanceIndex, 1);
}

void Stockpile::Update() {

}

void Stockpile::GiveWood() {
	storedWood++;
}

void Stockpile::GiveStone() {
	storedStone++;
}
