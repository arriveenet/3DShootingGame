#include "window.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GlU32.lib")

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        1,1,1,
        0,1,0,
        0, 1, 0);

    glBegin(GL_LINES);
    for (int i = -35; i < 36; i += 2) {
        glVertex3i(i, 0, -35);
        glVertex3i(i, 0, 35);
        glVertex3i(-35, 0, i);
        glVertex3i(35, 0, i);
    }
    glEnd();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    windowInit();
    windowCreate(L"title");
    windowDisplayFunc(display);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60.0,	// GLdouble fovy
        (GLdouble)WINDOW_WIDTH / WINDOW_HEIGHT,	// GLdouble aspect
        0.1,	// GLdouble zNear
        0.0);// GLdouble zFar

    windowMainLoop();

    return 0;
}