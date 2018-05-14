#include "InputController.h"
#include "GameController.h"

InputController::InputController(GameController* game, Camera* camera, ModelController* models, VertexBufferController* vbc): game(game), camera(camera), models(models), vbc(vbc) {
	controlPressed = isLeftMouseDown = false;

	D3DXMatrixIdentity(&tileRotation);
	D3DXMatrixTranspose(&tileRotation, &tileRotation);

	tileInstanceIndex = vbc->AddInstance({ tileScale, tileRotation, tileLocation });
	chevronInstancesIndex = vbc->AddInstance({ tileScale, tileRotation, tileLocation });
	chevronInstanceCount = 1;
}

InputController::~InputController() {

}

void InputController::Render() {
	if (isLeftMouseDown) {
		float currentX, currentY, startX, startY;
		GetMouseGridCoordinates(&currentX, &currentY);
		startX = dragStartX;
		startY = dragStartY;

		/*std::stringstream stream;
		stream << abs(currentX - dragStartX) << "," << abs(currentY - dragStartY) << std::endl;
		OutputDebugString(stream.str().c_str());*/

		if (currentX >= startX) {
			currentX = ceil(currentX);
			startX = floor(startX);
		}
		else {
			currentX = floor(currentX);
			startX = ceil(startX);
		}
		if (currentY >= startY) {
			currentY = ceil(currentY);
			startY = floor(startY);
		}
		else {
			currentY = floor(currentY);
			startY = ceil(startY);
		}

		D3DXMatrixScaling(&tileScale, abs(currentX - startX), 1.0f, abs(currentY - startY));
		D3DXMatrixTranslation(&tileLocation, (currentX + startX) / 2.0f, 0.0f, (currentY + startY) / 2.0f);

		D3DXMatrixTranspose(&tileScale, &tileScale);
		D3DXMatrixTranspose(&tileLocation, &tileLocation);

		vbc->UpdateInstance({ tileScale, tileRotation, tileLocation }, *tileInstanceIndex);
		vbc->RenderInstanced(models->GetIndexOfModel(SELECTION_TILE), models->GetModelSize(SELECTION_TILE), *tileInstanceIndex, 1);
	}

	int newSize = selected.size();

	ObjectData* objectData = new ObjectData[newSize];

	static float rotationStep = 0.0f;
	rotationStep += (float)D3DX_PI * 0.01f;
	if (rotationStep > 2.0f * (float)D3DX_PI)
	{
		rotationStep -= 2.0f * (float)D3DX_PI;
	}

	for (int i = 0; i < newSize; i++) {
		D3DXMatrixIdentity(&(objectData[i].scale));
		D3DXMatrixRotationYawPitchRoll(&(objectData[i].rotation), rotationStep, D3DX_PI, 0.0f);
		//D3DXMatrixIdentity(&(objectData[i].rotation));
		D3DXMatrixTranslation(&(objectData[i].location), selected[i]->GetX() + 0.5f, 2.0f, selected[i]->GetY() + 0.5f);

		D3DXMatrixTranspose(&(objectData[i].scale), &(objectData[i].scale));
		D3DXMatrixTranspose(&(objectData[i].rotation), &(objectData[i].rotation));
		D3DXMatrixTranspose(&(objectData[i].location), &(objectData[i].location));
	}

	// if we need more instances to store selection chevrons add more
	if (selected.size() > chevronInstanceCount) {
		vbc->DeleteMultipleInstances(chevronInstancesIndex, chevronInstanceCount);
		chevronInstancesIndex = vbc->AddMultipleInstances(objectData, newSize);
		chevronInstanceCount = newSize;
	}
	else {
		vbc->UpdateMultipleInstances(objectData, newSize, *chevronInstancesIndex);
	}

	delete[] objectData;

	vbc->RenderInstanced(models->GetIndexOfModel(SELECTED_CHEVRON), models->GetModelSize(SELECTED_CHEVRON), *chevronInstancesIndex, newSize);
}

void InputController::LeftMouseDown() {
	// record the location where the mouse was when first pressed
	GetMouseGridCoordinates(&(dragStartX), &(dragStartY));
	isLeftMouseDown = true;

	/*GameObject* gameObject = game->grid[resultX][resultY];
	if (gameObject) {
		for (auto it = selected.begin(); it != selected.end(); it++) {
			if (*it == gameObject) {
				selected.erase(it);
				goto found;
			}
		}
		if (selected.)
		found:
		selected.push_back(gameObject);
		game->DeleteObject(game->grid[resultX][resultY]);
	} else {
		controlPressed ? game->AddTree(resultX, resultY) : game->AddRock(resultX, resultY);
	}*/
}

void InputController::LeftMouseUp() {
	float resultX, resultY;
	GetMouseGridCoordinates(&resultX, &resultY);
	isLeftMouseDown = false;
	selected.clear();

	int minX, minY, maxX, maxY;
	if (resultX >= dragStartX) {
		minX = (int)dragStartX;
		maxX = (int)ceil(resultX);
	}
	else {
		minX = (int)resultX;
		maxX = (int)ceil(dragStartX);
	}
	if (resultY >= dragStartY) {
		minY = (int)dragStartY;
		maxY = (int)ceil(resultY);
	}
	else {
		minY = (int)resultY;
		maxY = (int)ceil(dragStartY);
	}

	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			GameObject* gameObject = game->grid[x][y];
			if (gameObject) {
				selected.push_back(gameObject);
			}
		}
	}
}

void InputController::RightMouseDown() {
	float resultX, resultY;
	GetMouseGridCoordinates(&resultX, &resultY);

}

void InputController::RightMouseUp() {
	float resultX, resultY;
	GetMouseGridCoordinates(&resultX, &resultY);

}

void InputController::KeyDown(WPARAM keycode) {
	switch (keycode) {
	case VK_LEFT: {
		controlPressed ? camera->SetRotating(CAM_LEFT, true) : camera->SetMoving(CAM_LEFT, true);
	} break;
	case VK_RIGHT: {
		controlPressed ? camera->SetRotating(CAM_RIGHT, true) : camera->SetMoving(CAM_RIGHT, true);
	} break;
	case VK_UP: {
		controlPressed ? camera->SetRotating(CAM_UP, true) : camera->SetMoving(CAM_UP, true);
	} break;
	case VK_DOWN: {
		controlPressed ? camera->SetRotating(CAM_DOWN, true) : camera->SetMoving(CAM_DOWN, true);
	} break;
	case VK_CONTROL:
	case VK_RCONTROL: {
		controlPressed = true;
	} break;
	}
}

void InputController::KeyUp(WPARAM keycode) {
	switch (keycode) {
	case VK_LEFT: {
		camera->SetRotating(CAM_LEFT, false);
		camera->SetMoving(CAM_LEFT, false);
	} break;
	case VK_RIGHT: {
		camera->SetRotating(CAM_RIGHT, false);
		camera->SetMoving(CAM_RIGHT, false);
	} break;
	case VK_UP: {
		camera->SetRotating(CAM_UP, false);
		camera->SetMoving(CAM_UP, false);
	} break;
	case VK_DOWN: {
		camera->SetRotating(CAM_DOWN, false);
		camera->SetMoving(CAM_DOWN, false);
	} break;
	case VK_CONTROL:
	case VK_RCONTROL: {
		controlPressed = false;
	} break;
	}
}

void InputController::Scroll(WPARAM wParam) {
	short distance = GET_WHEEL_DELTA_WPARAM(wParam);
	camera->Zoom((int)distance);
}

void InputController::GetMouseGridCoordinates(float* resultX, float* resultY) {
	// don't ask me how this works I knew when I wrote it then forgot 10 mins after
	POINT p;
	GetCursorPos(&p);

	float x = ((2.0f * (float)p.x) / (float)Renderer::SCREEN_WIDTH) - 1.0f;
	float y = -(((2.0f * (float)p.y) / (float)Renderer::SCREEN_HEIGHT) - 1.0f);

	D3DXMATRIX projection = game->GetRenderer()->GetProjectionMatrix();
	x = x / projection._11;
	y = y / projection._22;

	D3DXMATRIX view = game->GetCamera()->GetViewMatrix();
	D3DXVECTOR3 direction;
	direction.x = (x * view._11) + (y * view._12) + view._13;
	direction.y = (x * view._21) + (y * view._22) + view._23;
	direction.z = (x * view._31) + (y * view._32) + view._33;

	D3DXVECTOR3 origin = game->GetCamera()->GetPosition();
	float lineConst = (0 - origin.y) / direction.y;
	*resultX = lineConst * direction.x + origin.x;
	*resultY = lineConst * direction.z + origin.z;
}
