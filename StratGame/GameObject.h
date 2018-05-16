#pragma once

#include "Includes.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "ModelController.h"

class GameObject {
public:
	GameObject(VertexBufferController* vbc, int x, int y) : vbc(vbc), x(x), y(y) {};
	virtual ~GameObject() {};
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void ReceiveClick(int x, int y) {};

	inline int GetX() { return x; };
	inline int GetY() { return y; };

protected:
	int x, y;

	VertexBufferController* vbc;
	D3DXMATRIX scale, rotation, location;
};

