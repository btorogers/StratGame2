#pragma once

#include "Includes.h"
#include "GameController.h"
#include "VertexBufferController.h"
#include "ModelController.h"
#include "WorldController.h"

class GameObject {
public:
	GameObject(GameController* game, int x, int y);
	~GameObject();
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void ReceiveClick(int x, int y) {};

	int GetX();
	int GetY();

protected:
	int x, y;

	GameController* game;
	VertexBufferController* vbc;
	ModelController* models;
	WorldController* world;

	D3DXMATRIX scale, rotation, location;

	int* instanceIndex;
};

