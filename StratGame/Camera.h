#pragma once

#include "Includes.h"

// units per second
#define MOVE_SPEED 15.0f
// units per degree turned??
#define ZOOM_SPEED 0.025f
// radians per second
#define ROTATE_SPEED 1.0f

enum direction {
	CAM_LEFT = 0,
	CAM_RIGHT = 1,
	CAM_UP = 2,
	CAM_DOWN = 3
};

class Camera {
public:
	Camera();

	void SetPosition(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXMATRIX GetViewMatrix();

	void Render();
	void UpdatePosition();
	void UpdateRotation();
	void Zoom(int distance);
	void StepRotateAroundOrigin();

	void SetMoving(direction direction, bool state);
	void SetRotating(direction direction, bool state);

private:
	std::bitset<4> moving;
	std::bitset<4> rotating;

	D3DXVECTOR3 position, rotation;
	D3DXMATRIX viewMatrix, rotationMatrix;
	DWORD timePrev, timeCurrent;
	float timePassed;
};