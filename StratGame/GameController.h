#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "VertexBufferController.h"
#include "Cuboid.h"
#include "SquarePyramid.h"
#include "Sphere.h"
#include "GameObject.h"
#include "InputController.h"

class GameController {
public:
	GameController(HWND hWnd);
	~GameController();
	void ExecuteMain();
	void Tick();
	void RenderObjects();
	void AddRandomCuboid();
	void AddGameObject(int x, int y);
	void Quit();

	InputController* GetInputController();
	Renderer* GetRenderer();
	VertexBufferController* GetVertexBufferController();
	Camera* GetCamera();

	static const int gridX = 25;
	static const int gridY = 25;
private:
	VertexBufferController* vbc;
	Renderer* r;
	Camera* camera;
	InputController* input;

	std::vector<GameObject*> objects;
	bool running;
};

