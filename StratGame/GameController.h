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

	void MainLoop();
	void RenderLoop();

	void Tick();
	void RenderObjects();

	void AddRandomCuboid();
	void AddGameObject(int x, int y);

	void Quit();

	InputController* GetInputController();
	Renderer* GetRenderer();
	VertexBufferController* GetVertexBufferController();
	Camera* GetCamera();

	static const int GRID_X = 25;
	static const int GRID_Y = 25;
	static const int TICK_INTERVAL = 20;
private:
	VertexBufferController* vbc;
	Renderer* r;
	Camera* camera;
	InputController* input;

	std::thread gameThread, renderThread;

	std::vector<GameObject*> objects;
	bool running;
};

