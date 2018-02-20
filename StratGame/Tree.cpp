#include "Tree.h"

bool Tree::modelLoaded = false;
int Tree::indexOfModel = 0;

Tree::Tree(VertexBufferController* vbc, int x, int y): GameObject(vbc, x, y) {
	rotationStep = 0.0f;
	D3DXMatrixIdentity(&rotation);
	if (!modelLoaded) {
		Sphere s(0.0f, 0.75f, 0.0f, 0.5f, D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));
		Cuboid c2(0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, D3DXCOLOR(0.75f, 0.5f, 0.0f, 1.0f));
		indexOfModel = s.AddSelfForRendering(vbc, true);
		c2.AddSelfForRendering(vbc, true);
		modelLoaded = true;
	}
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&location);
	instanceIndex = vbc->AddInstance({ scale, rotation, location });

	Update();
}

Tree::~Tree() {
	//vbc->DeleteInstance(*instanceIndex);
}

void Tree::Render() {
	if (instanceIndex)
		vbc->RenderDynamic(indexOfModel, 996, *instanceIndex);
}

void Tree::Update() {
	rotationStep += (float)D3DX_PI * 0.01f;
	if (rotationStep > 2.0f * (float)D3DX_PI)
	{
		rotationStep -= 2.0f * (float)D3DX_PI;
	}
	D3DXMatrixTranslation(&location, (float)x + 0.5f, 0.5f, (float)y + 0.5f);
	D3DXMatrixRotationY(&this->rotation, rotationStep);
	D3DXMatrixTranspose(&scale, &scale);
	D3DXMatrixTranspose(&rotation, &rotation);
	D3DXMatrixTranspose(&location, &location);
	vbc->UpdateInstance({ scale, rotation, location }, *instanceIndex);
}
