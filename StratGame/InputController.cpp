#include "InputController.h"
#include "GameController.h"

InputController::InputController(GameController* game, Camera* camera): game(game), camera(camera) {
	controlPressed = false;
}

InputController::~InputController() {

}

void InputController::LeftMousePressed() {
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
	float resultX = lineConst * direction.x + origin.x;
	float resultY = lineConst * direction.z + origin.z;

	game->AddGameObject((int)resultX, (int)resultY);
}

void InputController::KeyDown(WPARAM keycode) {
	switch (keycode) {
	case VK_LEFT: {
		controlPressed ? camera->SetRotating(LEFT, true) : camera->SetMoving(LEFT, true);
	} break;
	case VK_RIGHT: {
		controlPressed ? camera->SetRotating(RIGHT, true) : camera->SetMoving(RIGHT, true);
	} break;
	case VK_UP: {
		controlPressed ? camera->SetRotating(UP, true) : camera->SetMoving(UP, true);
	} break;
	case VK_DOWN: {
		controlPressed ? camera->SetRotating(DOWN, true) : camera->SetMoving(DOWN, true);
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
		camera->SetRotating(LEFT, false); camera->SetMoving(LEFT, false);
	} break;
	case VK_RIGHT: {
		camera->SetRotating(RIGHT, false); camera->SetMoving(RIGHT, false);
	} break;
	case VK_UP: {
		camera->SetRotating(UP, false); camera->SetMoving(UP, false);
	} break;
	case VK_DOWN: {
		camera->SetRotating(DOWN, false); camera->SetMoving(DOWN, false);
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
