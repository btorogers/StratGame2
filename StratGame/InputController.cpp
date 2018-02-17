#include "InputController.h"
#include "GameController.h"

InputController::InputController(GameController* game) {
	this->game = game;
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
	D3DXVECTOR3 result;
	result.x = lineConst * direction.x + origin.x;
	result.y = 0;
	result.z = lineConst * direction.z + origin.z;

	game->AddGameObject((int)result.x, (int)result.z);
}
