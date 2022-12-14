#include "texture.h"
#include "utils.h"
#include "soil.h"

#pragma comment(lib,"soil.lib")//采用pragma comment添加lib文件

std::unordered_map<std::string, Texture*> Texture::mCachedTextures;

void Texture::Init(const char*imagePath, bool invertY, GLenum wrapMode)
{

	//use simple opengl image library to load texture
	unsigned int flags = SOIL_FLAG_POWER_OF_TWO;
	if (invertY)
	{
		flags |= SOIL_FLAG_INVERT_Y;
	}
	mPath = imagePath;
	mTextureID = SOIL_load_OGL_texture(imagePath, 0, 0, flags);
	if (wrapMode == GL_CLAMP_TO_EDGE)
	{
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return;


	//load image file from disk to memory
	int nFileSize = 0;
	unsigned char*imageFileContent = LoadFileContent(imagePath, nFileSize);
	if (imageFileContent == nullptr) {
		return;
	}
	//decode image
	int width = 0, height = 0;
	unsigned char* pixelData = DecodeBMP(imageFileContent, width, height);
	//generate an opengl texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);//绑定当前纹理状态
	//operation on current texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 对超出坐标的纹理取mod,超出的部分重复取值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//绑定内存中的数据到显存中
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete imageFileContent;

}

unsigned char * Texture::DecodeBMP(unsigned char * bmpFileData, int & width, int & height)
{
	if (0x4D42 == *((unsigned short*)bmpFileData))
	{
		int pixelDataOffset = *((int*)(bmpFileData + 10));
		width = *((int*)(bmpFileData + 18));
		height = *((int*)(bmpFileData + 22));
		unsigned char*pixelData = bmpFileData + pixelDataOffset;
		//bgr bgr bgr ....
		//rgb rgb rgb
		for (int i = 0; i < width*height * 3; i += 3)
		{
			unsigned char temp = pixelData[i];
			pixelData[i] = pixelData[i + 2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	else
	{
		return nullptr;
	}
}

Texture* Texture::LoadTexture(const char*imagePath, bool invertY, GLenum wrapMode)
{
	if (mCachedTextures.find(imagePath) != mCachedTextures.end())
	{
		return mCachedTextures[imagePath];
	}
	Texture*texture = new Texture;
	texture->Init(imagePath, invertY, wrapMode);
	mCachedTextures.insert(std::pair<std::string, Texture*>(imagePath, texture));
	return texture;
}

void Texture::UnloadTexture(Texture*texture)
{
	auto iter = mCachedTextures.find(texture->mPath.c_str());
	if (iter != mCachedTextures.end())
	{
		mCachedTextures.erase(iter);//缓存管理器内删除
		glDeleteTextures(1, &iter->second->mTextureID);//显卡内删除
		delete texture;//内存内删除
	}
}
