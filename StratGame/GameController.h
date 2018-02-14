#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "VertexBufferController.h"
#include "Cuboid.h"
#include "SquarePyramid.h"
#include "Sphere.h"
#include "GameObject.h"

class GameController
{
public:
	GameController(HWND hWnd);
	~GameController();
	void ExecuteMain();
	void Tick();
	void RenderObjects();
	void AddRandomCuboid();
	void Quit();

	static const int gridX = 25;
	static const int gridY = 25;
private:
	VertexBufferController* vbc;
	Renderer* r;

	std::vector<GameObject*> objects;
	bool running;
};

