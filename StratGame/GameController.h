#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "VertexBufferController.h"
#include "InputController.h"
#include "ModelController.h"
#include "WorldController.h"

class GameController {
public:
	GameController(HWND hWnd);
	~GameController();

	void MainLoop();
	void RenderLoop();

	void Tick();
	void RenderObjects();

	void AddRandomCuboid();

	void Quit();

	InputController* GetInputController();
	Renderer* GetRenderer();
	VertexBufferController* GetVertexBufferController();
	Camera* GetCamera();
	ModelController* GetModelController();
	WorldController* GetWorld();
private:
	VertexBufferController* vbc;
	Renderer* r;
	Camera* camera;
	InputController* input;
	ModelController* models;
	WorldController* world;

	std::thread gameThread, renderThread;

	bool running;
};

