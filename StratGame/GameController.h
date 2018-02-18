#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "VertexBufferController.h"
#include "Cuboid.h"
#include "SquarePyramid.h"
#include "Sphere.h"
#include "GameObject.h"
#include "InputController.h"

#define GRID_X 25
#define GRID_Y 25
#define TICK_INTERVAL 20

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
private:
	VertexBufferController* vbc;
	Renderer* r;
	Camera* camera;
	InputController* input;

	std::thread gameThread, renderThread;

	std::vector<GameObject*> objects;
	bool running;
};

