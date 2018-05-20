#pragma once

#include "Includes.h"
#include "ModelController.h"
#include "VertexBufferController.h"
#include "WorldController.h"

class GameController;
class Camera;
class GameObject;

class InputController {
public:
	InputController(GameController* game);
	~InputController();

	void Render();

	void LeftMouseDown();
	void LeftMouseUp();
	void RightMouseDown();
	void RightMouseUp();

	void KeyDown(WPARAM keycode);
	void KeyUp(WPARAM keycode);
	void Scroll(WPARAM wParam);
private:
	// input pointers are the address where the output will be put
	void GetMouseGridCoordinates(float* x, float* y);

	D3DXMATRIX tileScale, tileRotation, tileLocation;

	int* tileInstanceIndex;

	int* chevronInstancesIndex;
	int chevronInstanceCount;

	float dragStartX, dragStartY;

	bool controlPressed, isLeftMouseDown;

	std::vector<GameObject*> selected;

	GameController* game;
	ModelController* models;
	Camera* camera;
	VertexBufferController* vbc;
	WorldController* world;
};