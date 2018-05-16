#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "VertexBufferController.h"
#include "Cuboid.h"
#include "SquarePyramid.h"
#include "Sphere.h"
#include "InputController.h"
#include "Tree.h"
#include "Rock.h"
#include "ModelController.h"
#include "Unit.h"

class GameController {
public:
	GameController(HWND hWnd);
	~GameController();

	void MainLoop();
	void RenderLoop();

	void Tick();
	void RenderObjects();

	void AddRandomCuboid();
	void AddTree(int x, int y);
	void AddRock(int x, int y);
	void DeleteObject(GameObject* object);

	void Quit();

	InputController* GetInputController();
	Renderer* GetRenderer();
	VertexBufferController* GetVertexBufferController();
	Camera* GetCamera();
	ModelController* GetModelController();

	GameObject* grid[GRID_X][GRID_Y];
private:
	VertexBufferController* vbc;
	Renderer* r;
	Camera* camera;
	InputController* input;
	ModelController* models;

	std::thread gameThread, renderThread;

	std::vector<GameObject*> objects;
	bool running;

	std::mutex objectslock;
};

