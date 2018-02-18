#pragma once

#include "Includes.h"
#include "Camera.h"

class GameController;

class InputController {
public:
	InputController(GameController* game, Camera* camera);
	~InputController();

	void LeftMousePressed();
	void KeyDown(WPARAM keycode);
	void KeyUp(WPARAM keycode);
	void Scroll(WPARAM wParam);
private:
	bool controlPressed;

	GameController* game;
	Camera* camera;
};