#pragma once

#include "Includes.h"

class Camera {
public:
	Camera();

	void SetPosition(D3DXVECTOR3 position);
	void SetRotation(D3DXVECTOR3 rotation);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	void StepRotateAroundOrigin();

private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXMATRIX viewMatrix;
};