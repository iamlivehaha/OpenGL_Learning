#pragma once
#include "ggl.h"
#include "vector3f.h"

void SceneInit(int mViewportWidth, int mViewportHeight);
void SceneDraw();
void SceneDrawUI();
void OnButtonDown(int x,int y);
void OnButtonUp(int x, int y);
void OnKeyDown(char code);
void OnKeyUp(char code);
void OnMouseMove(float deltaX, float deltaY);
void RenderOneFrame(float deltaTime);
void DrawViewPort(int ViewPortStartX, int ViewPortStartY, Vector3f EyePos, Vector3f ViewCenter);

