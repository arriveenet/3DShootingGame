#include "window.h"

static HMENU	m_hMenu;
static RECT		m_rc;
static HWND		m_hWnd;
static HDC		m_hDC;
static HGLRC	m_hGLRC;
bool g_keys[256];
void(*displayFunc)(void);

LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

int windowInit()
{
	m_hInstance = NULL;

	return 0;
}

int windowCreate(const wchar_t* _title)
{
	if (m_hInstance == NULL)
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

	HICON hIcon = NULL;
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, szExePath, MAX_PATH);

	if (hIcon == NULL && m_hInstance == 0)
		hIcon = ExtractIcon(m_hInstance, szExePath, 0);

	// Register the window class
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = hIcon;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = m_windowClassName;

	if (!RegisterClass(&wndClass)) {
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(dwError);
	}

	m_rc;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;

	m_hMenu = NULL;

	int nDefaultWidth = 640;
	int nDefalutHeight = 480;
	SetRect(&m_rc, 0, 0, nDefaultWidth, nDefalutHeight);
	AdjustWindowRect(&m_rc,
		WS_OVERLAPPEDWINDOW,
		(m_hMenu != NULL) ? true : false);

	m_hWnd = CreateWindow(
		m_windowClassName,
		_title,
		WS_OVERLAPPEDWINDOW,
		x, y,
		(m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
		0,
		m_hMenu,
		m_hInstance,
		0
	);

	if (m_hWnd == NULL) {
		DWORD dwError = GetLastError();
		return HRESULT_FROM_WIN32(dwError);
	}

	m_hDC = GetDC(m_hWnd);

	return S_OK;
}

static void OnCreate(HWND hWnd)
{
	HDC hDC;
	int nPfdID;
	BOOL bResult;

	const PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL
		| PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,
		0,
		0
	};

	hDC = GetDC(hWnd);

	nPfdID = ChoosePixelFormat(hDC, &pfd);
	if (nPfdID == 0) {
		return;
	}

	bResult = SetPixelFormat(hDC, nPfdID, &pfd);
	if (bResult == FALSE) {
		return;
	}

	m_hGLRC = wglCreateContext(hDC);
	if (m_hGLRC == NULL) {
		return;
	}
	wglMakeCurrent(hDC, m_hGLRC);

	ReleaseDC(hWnd, hDC);
}

static LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_CREATE:
		OnCreate(hWnd);
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hDC;

		hDC = BeginPaint(hWnd, &ps);

		//displayFunc();

		EndPaint(hWnd, &ps);
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		break;
	case WM_CLOSE:
	{
		HMENU hMenu;
		hMenu = GetMenu(hWnd);
		if (hMenu != NULL)
		{
			DestroyMenu(hMenu);
		}
		DestroyWindow(hWnd);
		UnregisterClass(
			m_windowClassName,
			m_hInstance
		);
		return 0;
	}

	case WM_DESTROY:
		if (m_hGLRC != NULL) {
			wglDeleteContext(m_hGLRC);
		}
		ReleaseDC(m_hWnd, m_hDC);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT windowMainLoop()
{
	HRESULT hr = S_OK;
	if (!IsWindowVisible(m_hWnd))
		ShowWindow(m_hWnd, SW_SHOW);

	bool bGotMsg;
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message) {
		bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

		if (bGotMsg) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			HDC hDC = GetDC(m_hWnd);
			wglMakeCurrent(hDC, m_hGLRC);
			displayFunc();
			SwapBuffers(hDC);
			wglMakeCurrent(NULL, NULL);
			ReleaseDC(m_hWnd, hDC);
		}
	}

	return hr;
}

void windowDisplayFunc(void (*func)(void))
{
	displayFunc = func;
}