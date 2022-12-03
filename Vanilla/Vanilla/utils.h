#pragma once
#include "ggl.h"
#include <stdio.h>

unsigned char * LoadFileContent(const char *path, int &filesize);
unsigned char * DecodeBMP(unsigned char*bmpFileData, int&width, int&height);
GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum type);
GLuint CreateTexture2DFromBMP(const char *bmpPath);
GLuint CreateDisplayList(std::function<void()>foo);
GLuint CaptureScreen(int width, int height, std::function<void()> foo);
void SaveScreenPixelToFile(int width, int height, std::function<void()> foo, const char*filePath);
float GetFrameTime();
void DebugPrintf(const char* str);