#include "GameController.h"

#define Color(r,g,b) D3DXCOLOR(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f)
#define RANDCOL D3DXCOLOR(rand() / 32767.0f, rand() / 32767.0f, rand() / 32767.0f, 1.0f)
#define RANDINT(x) (rand() / 32767.0f * x)
#define RANDPOSNEG(x) (RANDINT(x*2)-x)

GameController::GameController(HWND hWnd) {
	running = true;
	r = new Renderer(hWnd, this);
	vbc = r->GetVertexBufferController();

	Cuboid c(0.0f, 0.0f, 0.0f, 2.0f, Color(150, 50, 50));
	Cuboid c1(2.0f, 0.0f, 0.0f, 1.0f, Color(50, 150, 50));
	Cuboid c2(0.0f, 2.0f, 0.0f, 1.0f, Color(50, 50, 150));
	SquarePyramid sp(-2.0f, 0.0f, 0.0f, 1.0f, 0.5f, Color(255, 255, 255));
	Sphere s(-4.0f, 0.0f, 0.0f, 0.5f, Color(255, 255, 255));

	c.AddSelfForRendering(vbc, false);
	c1.AddSelfForRendering(vbc, false);
	c2.AddSelfForRendering(vbc, false);
	sp.AddSelfForRendering(vbc, false);
	s.AddSelfForRendering(vbc, false);

	GameObject* g = new GameObject(r, 3, 0);
	objects.push_back(g);
	g = new GameObject(r, -3, 0);
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

void GameController::AddRandomCuboid() {
	Cuboid c(RANDPOSNEG(5), RANDPOSNEG(5), RANDPOSNEG(5), RANDINT(2), RANDINT(2), RANDINT(2), RANDCOL);
	c.AddSelfForRendering(vbc, false);
}