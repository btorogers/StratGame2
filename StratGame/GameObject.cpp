#include "GameObject.h"

GameObject::GameObject(Renderer* renderer, int x, int y): r(renderer), x(x), y(y) {
	vbc = r->GetVertexBufferController();
	D3DXMatrixIdentity(&rotation);
	if (!indexOfModel) {
		Cuboid c(0.0f, 0.0f, 0.0f, 2.0f, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));
		indexOfModel = new int{ c.AddSelfForRendering(vbc, true) };
	}
}

void GameObject::Render() {
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, (float)x, 0, (float)y);
	r->SetRotationMatrix(rotation);
	r->SetPositionMatrix(m);
	vbc->RenderDynamic(*indexOfModel, 36);
}

void GameObject::Update() {
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
	rotation -= 360.0f;
	}
	D3DXMatrixRotationY(&this->rotation, rotation);
	D3DXMATRIX m;
	D3DXMatrixRotationX(&m, rotation);
	D3DXMatrixMultiply(&this->rotation, &this->rotation, &m);
}
