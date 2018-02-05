#pragma once

#include <d3dx10math.h>

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);
	void StepRotateAroundOrigin();

private:
	int step;
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	D3DXMATRIX viewMatrix;
};