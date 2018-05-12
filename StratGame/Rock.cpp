#include "Rock.h"

bool Rock::modelLoaded = false;
int Rock::indexOfModel = 0;

Rock::Rock(VertexBufferController* vbc, int x, int y) : GameObject(vbc, x, y) {
	D3DXMatrixIdentity(&rotation);
	if (!modelLoaded) {
		Cuboid c(0.0f, -0.4f, 0.2f, 0.4f, 0.6f, 0.4f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		Cuboid c2(-0.25f, -0.5f, 0.0f, 0.2f, 0.3f, 0.2f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		Cuboid c3(0.25f, -0.5f, 0.0f, 0.5f, 0.15f, 0.5f, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		indexOfModel = c.AddSelfForRendering(vbc, true);
		c2.AddSelfForRendering(vbc, true);
		c3.AddSelfForRendering(vbc, true);
		modelLoaded = true;
	}
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&location);
	instanceIndex = vbc->AddInstance({ scale, rotation, location });

	Update();
}

Rock::~Rock() {
	vbc->DeleteInstance(instanceIndex);
}

void Rock::Render() {
	if (instanceIndex)
		vbc->RenderDynamic(indexOfModel, 108, *instanceIndex);
}

void Rock::Update() {
	D3DXMatrixTranslation(&location, (float)x + 0.5f, 0.5f, (float)y + 0.5f);
	D3DXMatrixTranspose(&scale, &scale);
	D3DXMatrixTranspose(&rotation, &rotation);
	D3DXMatrixTranspose(&location, &location);
	vbc->UpdateInstance({ scale, rotation, location }, *instanceIndex);
}
