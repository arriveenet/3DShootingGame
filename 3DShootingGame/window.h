#pragma once
#include <Windows.h>

int windowInit();
int windowCreate(const wchar_t* _title);
HRESULT windowMainLoop();
void windowDisplayFunc(void (*func)(void));

static HINSTANCE m_hInstance;
static const wchar_t* m_windowClassName = L"3DShootingGame";