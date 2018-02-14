#include "GameObject.h"

GameObject::GameObject(Renderer* renderer, int x, int y): r(renderer), x(x), y(y) {
	vbc = r->GetVertexBufferController();
	D3DXMatrixIdentity(&rotation);
	if (!indexOfModel) {
		Sphere s(0.0f, 0.0f, 0.0f, 0.5f, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f));
		//Cuboid c2(0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 1.5f, D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f));
		indexOfModel = new int{ s.AddSelfForRendering(vbc, true) };
		//c2.AddSelfForRendering(vbc, true);
	}
}

void GameObject::Render() {
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, (float)x + 0.5f, 0.5f, (float)y + 0.5f);
	r->SetRotationMatrix(rotation);
	r->SetPositionMatrix(m);
	vbc->RenderDynamic(*indexOfModel, 960);
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
