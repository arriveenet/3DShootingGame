#pragma once
#include <Windows.h>

int windowInit();
int windowCreate(const wchar_t* _title);
void windowPostRedisplay();
HRESULT windowMainLoop();
void windowDisplayFunc(void (*func)(void));
void windowIdleFunc(void (*func)(void));
void windowReshapeFunc(void(*func)(int width, int height));

static HINSTANCE m_hInstance;
static const wchar_t* m_windowClassName = L"3DShootingGame";

extern bool g_keys[256];