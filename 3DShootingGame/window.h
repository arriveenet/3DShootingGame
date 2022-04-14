#pragma once
#include <Windows.h>

int windowInit();
void windowInitSize(int width, int height);
void windowInitPosition(int x, int y);
int windowCreate(const char* _title);
void windowPostRedisplay();
void windowSwapBuffers();
HRESULT windowMainLoop();
void windowDisplayFunc(void (*func)(void));
void windowIdleFunc(void (*func)(void));
void windowReshapeFunc(void(*func)(int width, int height));
void windowReleaseFunc(void (*func)(void));

static HINSTANCE m_hInstance;
static const wchar_t* m_windowClassName = L"3DShootingGame";

extern bool g_keys[256];