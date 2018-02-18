#include "GameObject.h"

GameObject::GameObject(Renderer* renderer, int x, int y): r(renderer), x(x), y(y) {
	vbc = r->GetVertexBufferController();
	rotationStep = 0.0f;
	D3DXMatrixIdentity(&rotation);
	if (!indexOfModel) {
		Sphere s(0.0f, 0.0f, 0.0f, 0.5f, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));
		Cuboid c2(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 1.5f, D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f));
		indexOfModel = new int{ s.AddSelfForRendering(vbc, true) };
		c2.AddSelfForRendering(vbc, true);
	}
}

void GameObject::Render() {
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, (float)x + 0.5f, 0.5f, (float)y + 0.5f);
	r->SetRotationMatrix(rotation);
	r->SetPositionMatrix(m);
	vbc->RenderDynamic(*indexOfModel, 996);
}

void GameObject::Update() {
	rotationStep += (float)D3DX_PI * 0.01f;
	if (rotationStep > 2.0f * (float)D3DX_PI)
	{
	rotationStep -= 2.0f * (float)D3DX_PI;
	}
	D3DXMatrixRotationY(&this->rotation, rotationStep);
}
