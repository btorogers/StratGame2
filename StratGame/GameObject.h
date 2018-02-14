#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "Cuboid.h"
#include "Sphere.h"

class GameObject {
public:
	GameObject(Renderer* renderer, int x, int y);
	virtual ~GameObject() {};
	virtual void Render();
	virtual void Update();
private:
	int x, y;
	Renderer* r;
	VertexBufferController* vbc;
	int* indexOfModel;
	D3DXMATRIX rotation;
};

