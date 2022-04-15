#include "window.h"
#include "Header.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "font.h"
#include "frameCounter.h"

#include <glm/glm.hpp>
#include <stdio.h>
#include <time.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.lib")

using namespace glm;

ivec2 g_windowSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
vec3 position(-2, 2, -2);
vec3 center(1, 1, 1);

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
	position.x = sinf(radians(g_player.getRotation())) * 5.0f;
	position.y = 3.2f;
	position.z = cosf(radians(g_player.getRotation())) * 5.0f;
	position = g_player.getPosition() + position;
	center = g_player.getPosition();
	gluLookAt(
		position.x, position.y, position.z,
		center.x, center.y, center.z,
		0, 1, 0);

	g_field.draw();
	g_enemy.draw();
	g_player.draw();
	
	glColor3ub(0x00, 0xff, 0x00);
	fontBegin();
	{
		fontPosition(0, 0);
		fontDraw("FPS: %d\n", g_frameRate);
		fontDraw("Position: %f,%f,%f\n",
			g_player.getPosition().x,
			g_player.getPosition().y,
			g_player.getPosition().z);
		fontDraw("Rotation: %f\n", g_player.getRotation());
		fontDraw("Enemy_state: %s\n", enemy_state_string[g_enemy.m_state]);
	}
	fontEnd();

	windowSwapBuffers();
}

void idle()
{
	frameCounterUpdate();
	g_enemy.update();
	g_player.update();

	if (g_keys['R'])
		g_enemy.m_dead = false;

	if (g_keys[0x1b])
		exit(0);

	windowPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(
		0,// GLint x
		0,// GLint y
		width,// GLsizei width
		height);// GLsizei height

	g_windowSize = vec2(width, height);

	windowPostRedisplay();
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
	windowInitSize(g_windowSize.x, g_windowSize.y);
	windowInitPosition(100, 100);
	windowCreate("3D Shooting game");

	// Set callback function
	windowDisplayFunc(display);
	windowIdleFunc(idle);
	windowReshapeFunc(reshape);
	windowReleaseFunc(release);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60.0,	// GLdouble fovy
		(GLdouble)g_windowSize.x / g_windowSize.y,	// GLdouble aspect
		0.1,	// GLdouble zNear
		0.0);// GLdouble zFar

	frameCounterInit();
	fontInit();

	// Main loop
	windowMainLoop();

	return 0;
}