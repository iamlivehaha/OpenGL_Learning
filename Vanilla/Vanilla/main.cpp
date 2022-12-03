#include "ggl.h"
#include "scene.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "utils.h"
#include "camera.h"


#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"winmm.lib")

POINT originalPos;
bool rotateView = false;
static bool bPushingOnMe = false;
int mViewportWidth;
int mViewportHeight;

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam) - mViewportWidth / 2;
		int y = mViewportHeight / 2 - HIWORD(lParam);
		OnButtonDown(x, y);
	}
		break;
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam) - mViewportWidth / 2;
		int y = mViewportHeight / 2 - HIWORD(lParam);
		OnButtonUp(x, y);
	}
		break;
	case WM_RBUTTONDOWN:
		GetCursorPos(&originalPos);
		ShowCursor(false);
		rotateView = true;
		return 0;
	case WM_RBUTTONUP:
		SetCursorPos(originalPos.x, originalPos.y);
		ShowCursor(true);
		rotateView = false;
		return 0;
	case WM_MOUSEMOVE:
		if (rotateView) {
			POINT currentPos;
			GetCursorPos(&currentPos);
			float deltaX = currentPos.x - originalPos.x;
			float deltaY = currentPos.y - originalPos.y;
			OnMouseMove(deltaX, deltaY);
			SetCursorPos(originalPos.x, originalPos.y);
		}
		return 0;
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;
	case WM_KEYUP:
		OnKeyUp(wParam);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR CmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hIconSm = NULL;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = GLWindowProc;
	wndclass.lpszClassName = L"GLWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndclass);
	if (!atom) {
		MessageBox(NULL, L"Register Fail", L"Error", MB_OK);
		return 0;
	}
	RECT rect;
	rect.left = 0;
	rect.right = 1280;
	rect.top = 0;
	rect.bottom = 720;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);
	mViewportWidth = rect.right - rect.left;
	mViewportHeight = rect.bottom - rect.top;

	//create window
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"OpenGL Window", WS_OVERLAPPEDWINDOW,
		100, 100, mViewportWidth, mViewportHeight,
		NULL, NULL, hInstance, NULL);
	//create opengl render context
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormat, &pfd);
	
	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);//setup opengl context complete to device
	//init  opengl

	glViewport(0, 0, mViewportWidth, mViewportHeight);
	//setup MVP matrix transformation
	glMatrixMode(GL_PROJECTION);//tell gpu projection matrix is choosed
	//set value to projection matrix
	gluPerspective(50.0f, (float)mViewportWidth / (float)mViewportHeight, 0.1f, 1000.0f);// fov, resolv aspect,nearz,farz
	glMatrixMode(GL_MODELVIEW);// modelview matrix
	glEnable(GL_CULL_FACE);

	glLoadIdentity();

	SceneInit(mViewportWidth, mViewportHeight);



	//show window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);



	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//draw scene
		SceneDraw();
	

		//present scene
		SwapBuffers(dc);
	}
	return 0;
}