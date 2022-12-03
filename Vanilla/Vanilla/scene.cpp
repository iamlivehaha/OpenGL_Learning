#include "scene.h"
#include "ggl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "utils.h"
#include "skybox.h"
#include "Ground.h"
//#include "light.h"
#include "texture.h"
#include "objmodel.h"
#include "camera.h"
#include "ImageSprite.h"
#include "button.h"
#include "List.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"winmm.lib")

ObjModel model;
Camera camera;
Texture *objTexture;
SkyBox skybox;
ImageSprite sprite;
Ground ground;
Button* headButton;

void SceneInit(int mViewportWidth, int mViewportHeight)
{


	//init light
	float blackColor[] = { 0.0f,0.0f,0.0f,1.0f };
	float whiteColor[] = { 1.0f,1.0f,1.0f,1.0f };
	float lightPos[] = { 0.0f,1.0f,1.0f,0.0f };//
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);//direction light,point,spot

	float blackMat[] = { 0.0f,0.0f,0.0f,1.0f };
	float ambientMat[] = { 0.1f,0.1f,0.1f,1.0f };
	float diffuseMat[] = { 0.1f,0.3f,0.6f,1.0f };
	float specularMat[] = { 0.9f,0.9f,0.9f,1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//load texture

	skybox.Init("res/skybox");
	objTexture = Texture::LoadTexture("res/earth.bmp", true);//init opengl texture

	//set camera 
	camera.mViewportWidth = mViewportWidth;
	camera.mViewportHeight = mViewportHeight;

	//load model

	model.Init("res/Sphere.obj");

	//load ground
	ground.Init();

	//load UI
	Texture*spriteImage = Texture::LoadTexture("res/head.png");

	ImageSprite*sprite = new ImageSprite;
	sprite->SetTexture(spriteImage);
	headButton = new Button;
	headButton->SetDefaultSprite(sprite);
	headButton->SetRect(-580.0f, 300.0f, 100.0f, 100.0f);
	headButton->SetOnClick([]()->void {printf("i've been clicked\n"); });

	sprite = new ImageSprite;
	sprite->SetTexture(Texture::LoadTexture("res/fight.png"));
	Button*btn = new Button;
	btn->SetDefaultSprite(sprite);
	btn->SetRect(580.0f, 300.0f, 40.0f, 40.0f);
	btn->SetOnClick([]()->void {printf("fight!!\n"); });
	headButton->Push(btn);

	sprite = new ImageSprite;
	sprite->SetTexture(Texture::LoadTexture("res/icon.png"));
	btn = new Button;
	btn->SetDefaultSprite(sprite);
	btn->SetRect(480.0f, 300.0f, 100.0f, 100.0f);
	btn->SetOnClick([]()->void {printf("gift!!\n"); });
	headButton->Push(btn);

	////save screen shortcut
	//SaveScreenPixelToFile(mViewportWidth, mViewportHeight, []()->void
	//{
	//	RenderOneFrame(0.0f);
	//}, "screenshot.bmp");

}
void RenderOneFrame(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.SwitchTo3D(camera.mViewportWidth, camera.mViewportHeight);
	glLoadIdentity();
	//set up camera
	camera.Update(deltaTime);
	/*draw leftup viewport*/
	glViewport(0, 0, camera.mViewportWidth, camera.mViewportHeight);

	//draw skybox
	skybox.Draw(camera.mPos.x, camera.mPos.y, camera.mPos.z);

	//draw ground 
	ground.Draw();

	//draw model
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, objTexture->mTextureID);

	//draw mirror effect first
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glPushMatrix();
	glTranslatef(0.0f, -2.0f, 0.0f);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	model.Draw();

	//draw entity later
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	model.Draw();

	//draw UI
	SceneDrawUI();
	glLoadIdentity();
	headButton->Draw();
}

void DrawViewPort(int ViewPortStartX, int ViewPortStartY, Vector3f EyePos, Vector3f ViewCenter)
{
	/*draw leftup viewport*/
	glViewport(ViewPortStartX, ViewPortStartY, camera.mViewportWidth / 2, camera.mViewportHeight / 2);
	//gluLookAt(EyePos.x, EyePos.y, EyePos.z, ViewCenter.x, ViewCenter.y, ViewCenter.z, 0.0f, 1.0f, 0.0f);
	//draw skybox
	skybox.Draw(camera.mPos.x, camera.mPos.y, camera.mPos.z);

	//draw ground 
	ground.Draw();

	//draw model
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, objTexture->mTextureID);

	//draw mirror effect first
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glPushMatrix();
	glTranslatef(0.0f, -2.0f, 0.0f);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	model.Draw();

	//draw entity later
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	model.Draw();
}
void SceneDraw()
{
	int mViewportWidth = camera.mViewportWidth;
	int mViewportHeight = camera.mViewportHeight;
	//draw scene
	camera.SwitchTo3D(mViewportWidth, mViewportHeight);

	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 255.0f, 1.0f);//set clear black color for background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set up camera
	float frameTime = GetFrameTime();

	RenderOneFrame(frameTime);

	//camera.Update(frameTime);
	////printf("time elapse: %f\n", frameTime);

	///*draw leftup viewport*/
	//DrawViewPort(0, mViewportHeight / 2, Vector3f(0,0,5), Vector3f(0, 0, 4));



	///*draw rightup viewport*/
	//DrawViewPort(mViewportWidth / 2, mViewportHeight / 2, Vector3f(5, 0, 0), Vector3f(4, 0, 0));



	///*draw leftdown viewport*/
	//DrawViewPort(0, 0, Vector3f(-4, 3, 4), Vector3f(0, 0, 0));



	///*draw rightdown viewport*/
	//DrawViewPort(mViewportWidth / 2, 0, Vector3f(4, 3, 4), Vector3f(0, 0, 0));



	//glPointSize(10.0f);
	//glBegin(GL_POINTS);//start to draw
	//glColor4ub(255, 0, 0, 255); glVertex3f(-0.5f, -0.25f, -2.5f);
	//glColor4ub(0, 255, 0, 255); glVertex3f(0.0f, 0.5f, -2.5f);
	//glColor4ub(0, 0, 255, 255); glVertex3f(0.5f, -0.25f, -2.5f);
	//glEnd();

	//glLineWidth(10.0f);
	//glBegin(GL_LINE_LOOP);//start to draw
	//glColor4ub(255, 0, 0, 255); glVertex3f(-0.5f, -0.25f, -2.5f);
	//glColor4ub(0, 255, 0, 255); glVertex3f(0.0f, 0.5f, -2.5f);
	//glColor4ub(0, 0, 255, 255); glVertex3f(0.5f, -0.25f, -2.5f);

	//glPushMatrix();//压栈，将栈顶元素压一次，将元素的拷贝放置在栈顶。
	//矩阵还可以累加进行操作，不同的压栈顺序结果也不同
	//矩阵变换不具备乘法交换律，需要用四元数表达仿射变换（线性变换+平移）
	//glScalef(1.8f, 0.5f, 1.f);
	//glRotatef(20.f, 0.f, 0.f, 1.0f);//right hand rule  big thumb toward rotate(Z+) axis
	//glTranslatef(0.4, -0.4f, 0.f);

	//glBegin(GL_TRIANGLES);//start to draw something
	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(-1.0f, -0.5f, -2.0f);

	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(2.0f, 0.0f);
	//glVertex3f(1.0f, -0.5f, -2.0f);

	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(0.0, 2.0f);
	//glVertex3f(-1.0f, -0.5f, -3.0f);


	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(2.0f, 0.0f);
	//glVertex3f(1.0f, -0.5f, -2.0f);

	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(2.0f, 2.0f);
	//glVertex3f(1.0f, -0.5f, -3.0f);

	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glTexCoord2f(0.0, 2.0f);
	//glVertex3f(-1.0f, -0.5f, -3.0f);
	////glPopMatrix();
	//glEnd();
}

void SceneDrawUI()
{

	//draw ui
	camera.SwitchTo2D(camera.mViewportWidth, camera.mViewportHeight);
	glLoadIdentity();
	headButton->Draw();
}

void OnButtonDown(int x, int y)
{
	headButton->OnTouchBegin(x, y);
}

void OnButtonUp(int x, int y)
{

	headButton->OnTouchEnd(x, y);
	headButton->ResetState();
}

void OnKeyDown(char code) {
	switch (code)
	{
	case 'A':
		camera.mbMoveLeft = true;
		break;
	case 'D':
		camera.mbMoveRight = true;
		break;
	case 'W':
		camera.mbMoveBack = true;
		break;
	case 'S':
		camera.mbMoveForward = true;
		break;
	}
}
void OnKeyUp(char code) {

	switch (code)
	{
	case 'A':
		camera.mbMoveLeft = false;
		break;
	case 'D':
		camera.mbMoveRight = false;
		break;
	case 'W':
		camera.mbMoveBack = false;
		break;
	case 'S':
		camera.mbMoveForward = false;
		break;
	}
}

void OnMouseMove(float deltaX, float deltaY)
{
	float angleRotatedInX = deltaY / 1000.0f;
	float angleRotatedInY = deltaX / 1000.0f;
	camera.Yaw(angleRotatedInY);
	camera.Pitch(angleRotatedInX);
}
