#include "camera.h"
#include <windows.h>
#include "utils.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "vector3f.h"


Camera::Camera() :mPos(0.0f, 0.0f, 5.0f), mViewCenter(0.0f, 0.0f, 4.0f), mUp(0.0f, 1.0f, 0.0f)
{
	mbMoveLeft = false;
	mbMoveRight = false;
	mbMoveForward = false;
	mbMoveBack = false;
}

void Camera::Update(float deltaTime)
{
	//update everything
	float moveSpeed = 3.0f;

	//if (!mbMoveLeft&&!mbMoveRight&&!mbMoveForward&&!mbMoveBack)
	//{
	//	DebugPrintf("move stoped");
	//}

	if (mbMoveLeft) {
		float delta = deltaTime * moveSpeed;
		mPos.x = mPos.x + delta;
		mViewCenter.x = mViewCenter.x + delta;
	}
	if (mbMoveRight) {
		float delta = deltaTime * moveSpeed;
		mPos.x = mPos.x - delta;
		mViewCenter.x = mViewCenter.x - delta;
	}
	if (mbMoveForward) {
		float delta = deltaTime * moveSpeed;
		mPos.z = mPos.z + delta;
		mViewCenter.z = mViewCenter.z + delta;
	}
	if (mbMoveBack) {
		float delta = deltaTime * moveSpeed;
		mPos.z = mPos.z - delta;
		mViewCenter.z = mViewCenter.z - delta;
	}

	//set model view matrix
	glLoadIdentity();//clear last frame move matrix
	gluLookAt(mPos.x, mPos.y, mPos.z,
		mViewCenter.x, mViewCenter.y, mViewCenter.z,
		mUp.x, mUp.y, mUp.z);
}

void Camera::Pitch(float angle) {
	Vector3f viewDirection = mViewCenter - mPos;
	viewDirection.Normalize();
	Vector3f rightDirection = viewDirection.Cross(mUp);
	rightDirection.Normalize();
	RotateView(angle, rightDirection.x, rightDirection.y, rightDirection.z);
}
void Camera::Yaw(float angle) {
	RotateView(angle, mUp.x, mUp.y, mUp.z);
}

void Camera::RotateView(float angle, float x, float y, float z)
{
	Vector3f viewDirection = mViewCenter - mPos;
	Vector3f newDirection;
	float C = cosf(angle);
	float S = sinf(angle);

	Vector3f tempX(C + x * x*(1 - C), x*y*(1 - C) - z * S, x*z*(1 - C) + y * S);
	newDirection.x = tempX * viewDirection;

	Vector3f tempY(x*y*(1 - C) + z * S, C + y * y*(1 - C), y*z*(1 - C) - x * S);
	newDirection.y = tempY * viewDirection;

	Vector3f tempZ(x*z*(1 - C) - y * S, y*z*(1 - C) + x * S, C + z * z*(1 - C));
	newDirection.z = tempZ * viewDirection;

	mViewCenter = mPos + newDirection;
}

void Camera::SwitchTo3D(int mViewportWidth, int mViewportHeight)
{
	glMatrixMode(GL_PROJECTION);//tell the gpu processer that i would select the projection matrix
	glLoadIdentity();
	gluPerspective(50.0f, (float)mViewportWidth / (float)mViewportHeight, 0.1f, 1000.0f);//set some values to projection matrix
	glMatrixMode(GL_MODELVIEW);//tell .... model view matrix
}

void Camera::SwitchTo2D(int mViewportWidth, int mViewportHeight)
{
	glMatrixMode(GL_PROJECTION);//tell the gpu processer that i would select the projection matrix
	glLoadIdentity();
	gluOrtho2D(-mViewportWidth / 2, mViewportWidth / 2, -mViewportHeight / 2, mViewportHeight / 2);
	glMatrixMode(GL_MODELVIEW);//tell .... model view matrix
}
