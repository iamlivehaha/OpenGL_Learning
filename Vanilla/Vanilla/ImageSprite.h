#pragma once
#include <windows.h>
#include <gl/GL.h>
#include "texture.h"
#include "vector3f.h"

class ImageSprite
{
public:
	Texture*mTexture;
	Vector3f mMesh[4];
	void SetTexture(Texture*texture);
	void SetRect(float x, float y, float width, float height);
	void Draw();
};