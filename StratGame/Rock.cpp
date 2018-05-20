#include "Rock.h"

Rock::Rock(GameController* game, int x, int y) : GameObject(game, x, y) {

}

void Rock::Render() {
	if (instanceIndex)
		vbc->RenderInstanced(models->GetIndexOfModel(ModelID::ROCK), models->GetModelSize(ModelID::ROCK), *instanceIndex, 1);
}

void Rock::Update() {
	
}
