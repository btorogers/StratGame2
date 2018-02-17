#pragma once

#include "Includes.h"

class GameController;

class InputController {
public:
	InputController(GameController* game);
	~InputController();

	void LeftMousePressed();
private:
	GameController* game;
};