#include "Camera.h"


Camera::Camera() {
	position.x = 5.0f;
	position.y = 20.0f;
	position.z = -10.0f;

	// -left +right
	rotation.x = 0.0f;
	// -up +down
	rotation.y = 0.0f;
	// +rollleft -rollright
	rotation.z = 0.0f;
}

void Camera::SetPosition(D3DXVECTOR3 pos) {
	position = pos;
}

void Camera::SetRotation(D3DXVECTOR3 rot) {
	rotation = rot;
}

D3DXVECTOR3 Camera::GetPosition() {
	return position;
}

D3DXVECTOR3 Camera::GetRotation() {
	return rotation;
}

void Camera::Render() {
	D3DXVECTOR3 up, lookAt;
	D3DXMATRIX rotationMatrix;

	//StepRotateAroundOrigin();

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	lookAt.x = 0.0f;
	lookAt.y = -1.0f;
	lookAt.z = 1.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.x, rotation.y, rotation.z);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}

void Camera::StepRotateAroundOrigin() {
	static float step = 0.0f;
	if (step >= D3DX_PI * 2) {
		step = 0;
	}
	position.x = (float)(10 * cos(step));
	position.z = (float)(10 * sin(step));
	step += (float)D3DX_PI * 0.005f;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = this->viewMatrix;
}