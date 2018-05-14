#include "GameController.h"

#define Color(r,g,b) D3DXCOLOR(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f)
#define RANDCOL D3DXCOLOR(rand() / 32767.0f, rand() / 32767.0f, rand() / 32767.0f, 1.0f)
#define RANDINT(x) (rand() / 32767.0f * x)
#define RANDPOSNEG(x) (RANDINT(x*2)-x)

GameController::GameController(HWND hWnd) {
	running = true;
	r = new Renderer(hWnd, this);
	vbc = r->GetVertexBufferController();
	vbc->GenerateGrid(GRID_X, GRID_Y);

	camera = r->GetCamera();
	models = new ModelController(vbc);
	input = new InputController(this, camera, models, vbc);

	Cuboid c((float)GRID_X / 2.0f, -0.05f, (float)GRID_Y / 2.0f, (float)GRID_X, 0.0499f, (float)GRID_X, Color(100, 200, 100), vbc);

	c.AddSelfForRendering(false);

	AddTree(12, 2);
	AddTree(15, 13);
	AddRock(4, 15);
	AddRock(8, 5);

	gameThread = std::thread(&GameController::MainLoop, this);
	renderThread = std::thread(&GameController::RenderLoop, this);
}

GameController::~GameController() {
	for (auto it = objects.begin(); it != objects.end(); it++) {
		delete *it;
	}
	delete r;
	objects.clear();
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
	objectslock.lock();
	for (GameObject* o : objects) {
		o->Update();
	}
	objectslock.unlock();
}

void GameController::RenderObjects() {
	objectslock.lock();
	for (GameObject* o : objects) {
		o->Render();
	}
	objectslock.unlock();
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

inline ModelController* GameController::GetModelController()
{
	return models;
}

void GameController::AddRandomCuboid() {
	Cuboid c(RANDPOSNEG(5), RANDPOSNEG(5), RANDPOSNEG(5), RANDINT(2), RANDINT(2), RANDINT(2), RANDCOL, vbc);
	c.AddSelfForRendering(false);
}

void GameController::AddTree(int x, int y) {
	objectslock.lock();
	GameObject* g = new Tree(vbc, x, y);
	objects.push_back(g);
	grid[x][y] = g;
	objectslock.unlock();
}

void GameController::AddRock(int x, int y) {
	objectslock.lock();
	GameObject* g = new Rock(vbc, x, y);
	objects.push_back(g);
	grid[x][y] = g;
	objectslock.unlock();
}

void GameController::DeleteObject(GameObject* object) {
	objectslock.lock();
	for (auto it = objects.begin(), end = objects.end(); it != end; it++) {
		if (*it == object) {
			objects.erase(it);
			break;
		}
	}
	grid[object->GetX()][object->GetY()] = 0;
	delete object;
	objectslock.unlock();
}
