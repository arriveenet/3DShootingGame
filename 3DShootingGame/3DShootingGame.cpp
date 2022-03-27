#include "window.h"
#include "Header.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include <time.h>

#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "font.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.lib")

using namespace glm;

vec3 position(-2, 2, -2);
vec3 center(1, 1, 1);
Player player;
clock_t startTime;
tm currentTime;
unsigned int FPS;
unsigned int frameCounter;

void xyzAxes(const vec3& _v, double length)
{
	glBegin(GL_LINES);
	{
		glColor3d(0xff, 0x00, 0x00);	// x-axis
		glVertex3d(_v.x, _v.y, _v.z);	glVertex3d(_v.x + length, _v.y, _v.z);
		glColor3d(0x00, 0xff, 0x00);	// y-axis
		glVertex3d(_v.x, _v.y, _v.z);	glVertex3d(_v.x, _v.y + length, _v.z);
		glColor3d(0x00, 0x00, 0xff);	// z-axis
		glVertex3d(_v.x, _v.y, _v.z);	glVertex3d(_v.x, _v.y, _v.z + length);
	}
	glEnd();
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	position.x = sinf(radians(player.m_rotation)) * 4.0f;
	position.y = 2.0f;
	position.z = cosf(radians(player.m_rotation)) * 4.0f;
	position = player.m_position + position;
	center = player.m_position;
	gluLookAt(
		position.x, position.y, position.z,
		center.x, center.y, center.z,
		0, 1, 0);

	g_field.draw();
	xyzAxes(vec3(0, 0, 0), 2);
	g_enemy.draw();
	player.draw();
}

void idle()
{
	if (clock() - startTime >= 1000) {
		startTime = clock();
		FPS = frameCounter;
		//printf("FPS:%d\n", FPS);
		frameCounter = 0;
	}
	frameCounter++;

	g_enemy.update();
	player.update();

	if (g_keys[0x1b])
		exit(0);
}

void reshape(int width, int height)
{
	glViewport(
		0,// GLint x
		0,// GLint y
		width,// GLsizei width
		height);// GLsizei height
}

void release()
{
	fontRelease();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	windowInit();
	windowCreate(L"title");

	// Set callback function
	windowDisplayFunc(display);
	windowIdleFunc(idle);
	windowReshapeFunc(reshape);
	windowReleaseFunc(release);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60.0,	// GLdouble fovy
		(GLdouble)WINDOW_WIDTH / WINDOW_HEIGHT,	// GLdouble aspect
		0.1,	// GLdouble zNear
		0.0);// GLdouble zFar

	errno_t err;
	time_t t = time(NULL);
	err = localtime_s(&currentTime, &t);
	startTime = clock();
	fontInit("font/font.fnt");

	// Main loop
	windowMainLoop();

	return 0;
}