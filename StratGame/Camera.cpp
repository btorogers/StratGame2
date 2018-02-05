#include "camera.h"


Camera::Camera() {
	positionX = 10.0f;
	positionY = 10.0f;
	positionZ = -10.0f;

	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;

	step = 0;
}

Camera::Camera(const Camera& other) {

}

Camera::~Camera() {

}

void Camera::SetPosition(float x, float y, float z) {
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Camera::SetRotation(float x, float y, float z) {
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

D3DXVECTOR3 Camera::GetPosition() {
	return D3DXVECTOR3(positionX, positionY, positionZ);
}

D3DXVECTOR3 Camera::GetRotation() {
	return D3DXVECTOR3(rotationX, rotationY, rotationZ);
}

void Camera::Render() {
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	StepRotateAroundOrigin();

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = positionX;
	position.y = positionY;
	position.z = positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotationX * 0.0174532925f;
	yaw   = rotationY * 0.0174532925f;
	roll  = rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	//lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
}

void Camera::StepRotateAroundOrigin() {
	if (step > 359) {
		step = 0;
	}
	positionX = 10 * cos(step * D3DX_PI / 180);
	positionZ = 10 * sin(step * D3DX_PI / 180);
	step++;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix) {
	viewMatrix = this->viewMatrix;
}