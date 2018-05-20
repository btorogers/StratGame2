#include "GameController.h"

GameController::GameController(HWND hWnd) {
	running = true;
	r = new Renderer(hWnd, this);
	vbc = r->GetVertexBufferController();
	vbc->GenerateGrid(GRID_X, GRID_Y);

	camera = r->GetCamera();
	models = new ModelController(vbc);
	world = new WorldController(this);
	input = new InputController(this);
	world->Generate();

	Cuboid c((float)GRID_X / 2.0f, -0.05f, (float)GRID_Y / 2.0f, (float)GRID_X, 0.0499f, (float)GRID_X, COLOR(100, 200, 100), vbc);
	c.AddSelfForRendering(false);

	gameThread = std::thread(&GameController::MainLoop, this);
	renderThread = std::thread(&GameController::RenderLoop, this);
}

GameController::~GameController() {
	delete r;
	running = false;
	gameThread.join();
	renderThread.join();
}

void GameController::MainLoop() {
	while (running) {
		static DWORD timePrev = GetTickCount();
		static DWORD timeCurrent = GetTickCount();

		timeCurrent = GetTickCount();
		if (timeCurrent > timePrev + TICK_INTERVAL) {
			timePrev = timeCurrent;
			Tick();
		}
	}
}

void GameController::RenderLoop() {
	while (running) {
		r->RenderFrame();
	}
}

void GameController::Tick() {
	world->UpdateAll();
}

void GameController::RenderObjects() {
	world->RenderAll();
}

void GameController::Quit() {
	running = false;
}

InputController* GameController::GetInputController() {
	return input;
}

Renderer* GameController::GetRenderer() {
	return r;
}

VertexBufferController* GameController::GetVertexBufferController() {
	return vbc;
}

Camera* GameController::GetCamera() {
	return camera;
}

ModelController* GameController::GetModelController() {
	return models;
}

WorldController * GameController::GetWorld() {
	return world;
}

void GameController::AddRandomCuboid() {
	Cuboid c(RANDPOSNEG(5), RANDPOSNEG(5), RANDPOSNEG(5), RANDINT(2), RANDINT(2), RANDINT(2), RANDCOL, vbc);
	c.AddSelfForRendering(false);
}
