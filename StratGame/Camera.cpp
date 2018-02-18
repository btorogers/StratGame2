#include "Camera.h"

Camera::Camera() {
	position.x = 5.0f;
	position.y = 20.0f;
	position.z = -10.0f;

	// -left +right
	rotation.x = (float)D3DX_PI / 4.0f;
	// -up +down
	rotation.y = (float)D3DX_PI / 4.0f;
	// +rollleft -rollright
	rotation.z = 0.0f;

	moving.reset();
	rotating.reset();

	timePrev = timeCurrent = GetTickCount();
}

void Camera::Render() {
	D3DXVECTOR3 up, lookAt;

	//StepRotateAroundOrigin();

	timeCurrent = GetTickCount();
	timePassed = (int)(timeCurrent - timePrev) / 1000.0f;
	UpdatePosition();
	UpdateRotation();
	timePrev = timeCurrent;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.x, rotation.y, rotation.z);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt += position;

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}

void Camera::UpdatePosition() {
	float dist = MOVE_SPEED * timePassed;

	D3DXMATRIX currentRotation;
	D3DXMatrixRotationY(&currentRotation, rotation.x);
	if (moving.test(0)) {
		D3DXVECTOR3 x(-dist, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&x, &x, &currentRotation);
		position += x;
	}
	if (moving.test(1)) {
		D3DXVECTOR3 x(dist, 0.0f, 0.0f);
		D3DXVec3TransformCoord(&x, &x, &currentRotation);
		position += x;
	}
	if (moving.test(2)) {
		D3DXVECTOR3 x(0.0f, 0.0f, dist);
		D3DXVec3TransformCoord(&x, &x, &currentRotation);
		position += x;
	}
	if (moving.test(3)) {
		D3DXVECTOR3 x(0.0f, 0.0f, -dist);
		D3DXVec3TransformCoord(&x, &x, &currentRotation);
		position += x;
	}
}

void Camera::UpdateRotation() {
	float dist = ROTATE_SPEED * timePassed;
	if (rotating.test(0)) {
		rotation.x -= dist;
	}
	if (rotating.test(1)) {
		rotation.x += dist;
	}

	if (rotation.x > D3DX_PI) {
		rotation.x -= 2 * D3DX_PI;
	}else if (rotation.x < -D3DX_PI) {
		rotation.x += 2 * D3DX_PI;
	}

	if (rotating.test(2)) {
		rotation.y -= dist;
	}
	if (rotating.test(3)) {
		rotation.y += dist;
	}

	if (rotation.y > D3DX_PI) {
		rotation.y -= 2 * D3DX_PI;
	}
	else if (rotation.y < -D3DX_PI) {
		rotation.y += 2 * D3DX_PI;
	}
}

void Camera::Zoom(int distance) {
	D3DXVECTOR3 x(0.0f, 0.0f, ZOOM_SPEED * distance);
	D3DXVec3TransformCoord(&x, &x, &rotationMatrix);
	position += x;
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

void Camera::SetMoving(direction direction, bool state) {
	moving.set(direction, state);
}

void Camera::SetRotating(direction direction, bool state) {
	rotating.set(direction, state);
}

void Camera::SetPosition(D3DXVECTOR3 position) {
	this->position = position;
}

void Camera::SetRotation(D3DXVECTOR3 rotation) {
	this->rotation = rotation;
}

D3DXVECTOR3 Camera::GetPosition() {
	return position;
}

D3DXVECTOR3 Camera::GetRotation() {
	return rotation;
}

D3DXMATRIX Camera::GetViewMatrix() {
	return viewMatrix;
}

