#include "GameController.h"

#define Color(r,g,b) D3DXCOLOR(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f)
#define RANDCOL D3DXCOLOR(rand() / 32767.0f, rand() / 32767.0f, rand() / 32767.0f, 1.0f)
#define RANDINT(x) (rand() / 32767.0f * x)
#define RANDPOSNEG(x) (RANDINT(x*2)-x)

GameController::GameController(HWND hWnd) {
	running = true;
	r = new Renderer(hWnd, this);
	input = new InputController(this);
	vbc = r->GetVertexBufferController();
	camera = r->GetCamera();

	vbc->GenerateGrid(gridX, gridY);

	Cuboid c(-4.0f, 0.0f, 0.0f, 2.0f, Color(150, 50, 50));
	Cuboid c1(-2.0f, 0.0f, 0.0f, 1.0f, Color(50, 150, 50));
	Cuboid c2(-4.0f, 2.0f, 0.0f, 1.0f, Color(50, 50, 150));
	SquarePyramid sp(-4.0f, 0.0f, 0.0f, 1.0f, 0.5f, Color(255, 255, 255));
	Sphere s(-6.0f, 0.0f, 0.0f, 0.5f, Color(255, 255, 255));

	c.AddSelfForRendering(vbc, false);
	c1.AddSelfForRendering(vbc, false);
	c2.AddSelfForRendering(vbc, false);
	sp.AddSelfForRendering(vbc, false);
	s.AddSelfForRendering(vbc, false);

	GameObject* g = new GameObject(r, 12, 2);
	objects.push_back(g);
	g = new GameObject(r, 15, 13);
	objects.push_back(g);
}

GameController::~GameController() {
	delete r;
	delete[objects.size()] objects[0];
	objects.clear();
}

void GameController::ExecuteMain() {
	static DWORD timePrev = GetTickCount();
	static DWORD timeCurrent = GetTickCount();

	timeCurrent = GetTickCount();
	if (timeCurrent > timePrev + 5) {
		timePrev = timeCurrent;
		Tick();
	}

	r->RenderFrame();
}

void GameController::Tick() {
	for (GameObject* o : objects) {
		o->Update();
	}
}

void GameController::RenderObjects() {
	for (GameObject* o : objects) {
		o->Render();
	}
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

void GameController::AddRandomCuboid() {
	Cuboid c(RANDPOSNEG(5), RANDPOSNEG(5), RANDPOSNEG(5), RANDINT(2), RANDINT(2), RANDINT(2), RANDCOL);
	c.AddSelfForRendering(vbc, false);
}

void GameController::AddGameObject(int x, int y) {
	GameObject* g = new GameObject(r, x, y);
	objects.push_back(g);
}
