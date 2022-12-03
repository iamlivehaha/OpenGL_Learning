#pragma once
#include "VertexData.h"
#include <windows.h>
#include <gl/GL.h>
#include "displaylist.h"

class ObjModel
{
public:
	DisplayList mObjModel;
	VertexData *mVertexes;
	int *mIndexs;
	int mIndexCount;

	void Init(const char*objModel);
	void Draw();
};