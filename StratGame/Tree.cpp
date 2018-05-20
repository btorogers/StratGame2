#include "Tree.h"

Tree::Tree(GameController* game, int x, int y): GameObject(game, x, y) {

}

void Tree::Render() {
	if (instanceIndex)
		vbc->RenderInstanced(models->GetIndexOfModel(ModelID::TREE), models->GetModelSize(ModelID::TREE), *instanceIndex, 1);
}

void Tree::Update() {
	/*rotationStep += (float)D3DX_PI * 0.01f;
	if (rotationStep > 2.0f * (float)D3DX_PI)
	{
		rotationStep -= 2.0f * (float)D3DX_PI;
	}
	D3DXMatrixTranslation(&location, (float)x + 0.5f, 0.5f, (float)y + 0.5f);
	D3DXMatrixRotationY(&this->rotation, rotationStep);
	D3DXMatrixTranspose(&scale, &scale);
	D3DXMatrixTranspose(&rotation, &rotation);
	D3DXMatrixTranspose(&location, &location);
	vbc->UpdateInstance({ scale, rotation, location }, *instanceIndex);*/
}
