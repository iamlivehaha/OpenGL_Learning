#pragma once
#include "vector3f.h"

class Camera
{
public:
	Camera();

	Vector3f mPos, mViewCenter, mUp;
	bool mbMoveLeft, mbMoveRight, mbMoveForward, mbMoveBack;
	int mViewportWidth, mViewportHeight;

	void Update(float deltaTime);
	void Pitch(float angle);
	void Yaw(float angle);
	void SwitchTo3D(int mViewportWidth, int mViewportHeight);
	void SwitchTo2D(int mViewportWidth, int mViewportHeight);
private:

	void RotateView(float angle, float x, float y, float z);
};