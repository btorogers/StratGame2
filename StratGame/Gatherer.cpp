#include "Gatherer.h"

Gatherer::Gatherer(GameController* game, int x, int y): Unit(game, x, y) {

}

void Gatherer::Render() {
	if (instanceIndex)
		vbc->RenderInstanced(models->GetIndexOfModel(ModelID::GATHERER), models->GetModelSize(ModelID::GATHERER), *instanceIndex, 1);
}

void Gatherer::Update() {
	Unit::Update();
}

void Gatherer::ReceiveClick(int x, int y) {
	Unit::ReceiveClick(x, y);
}
