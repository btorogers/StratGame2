#include "GameObject.h"

GameObject::GameObject(GameController* game, int x, int y): game(game), x(x), y(y) {
	vbc = game->GetVertexBufferController();
	models = game->GetModelController();
	world = game->GetWorld();

	world->grid[x][y] = this;

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&scale);
	D3DXMatrixTranslation(&location, x + 0.5f, 0.5f, y + 0.5f);

	D3DXMatrixTranspose(&scale, &scale);
	D3DXMatrixTranspose(&rotation, &rotation);
	D3DXMatrixTranspose(&location, &location);

	instanceIndex = vbc->AddInstance({ scale, rotation, location });
}

GameObject::~GameObject() {
	vbc->DeleteInstance(instanceIndex);
	delete instanceIndex;
}
;

int GameObject::GetX() {
	return x;
};

int GameObject::GetY() {
	return y;
}
