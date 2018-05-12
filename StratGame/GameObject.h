#pragma once

#include "Includes.h"
#include "Cuboid.h"
#include "Sphere.h"

class GameObject {
public:
	GameObject(VertexBufferController* vbc, int x, int y) : vbc(vbc), x(x), y(y) {};
	virtual ~GameObject() {};
	virtual void Render() = 0;
	virtual void Update() = 0;

	int GetX() { return x; };
	int GetY() { return y; };

protected:
	int x, y;
	VertexBufferController* vbc;
	D3DXMATRIX scale, rotation, location;
};

