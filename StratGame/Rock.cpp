#include "Rock.h"

bool Rock::modelLoaded = false;
int Rock::indexOfModel = 0;

Rock::Rock(VertexBufferController* vbc, int x, int y) : GameObject(vbc, x, y) {
	D3DXMatrixIdentity(&rotation);
	if (!modelLoaded) {
		Cuboid c(0.0f, -0.4f, 0.2f, 0.4f, 0.6f, 0.4f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f), vbc);
		Cuboid c2(-0.25f, -0.5f, 0.0f, 0.2f, 0.3f, 0.2f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f), vbc);
		Cuboid c3(0.25f, -0.5f, 0.0f, 0.5f, 0.15f, 0.5f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f), vbc);
		indexOfModel = c.AddSelfForRendering(true);
		c2.AddSelfForRendering(true);
		c3.AddSelfForRendering(true);
		modelLoaded = true;
	}
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixTranslation(&location, x + 0.5f, 0.5f, y + 0.5f);

	D3DXMatrixTranspose(&scale, &scale);
	D3DXMatrixTranspose(&rotation, &rotation);
	D3DXMatrixTranspose(&location, &location);

	instanceIndex = vbc->AddInstance({ scale, rotation, location });
	// vbc->UpdateInstance({ scale, rotation, location }, *instanceIndex);
}

Rock::~Rock() {
	vbc->DeleteInstance(instanceIndex);
}

void Rock::Render() {
	if (instanceIndex)
		vbc->RenderInstanced(indexOfModel, 108, *instanceIndex, 1);
}

void Rock::Update() {
	
}
