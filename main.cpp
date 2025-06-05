
// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "main.h"
#include "manager.h"
#include <thread>


const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_Window;
static float mouseWheelDelta;
static DWORD lastWheelTime = 0;  // 最後にホイールが動いた時間

HWND GetWindow()
{
	return g_Window;
}

float GetMouseWheelDelta()
{
	return mouseWheelDelta;
}

POINT MousePoint;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);


	Manager::GetInstance()->Init();
	/*Manager::Init();*/



	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	

	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	

	MSG msg;
	while(1)
	{
		
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

                Manager::GetInstance()->Update();
				Manager::GetInstance()->Draw();
				DWORD currentTime = GetTickCount64();

				//200ms 経過したらリセット
				if (currentTime - lastWheelTime > 200)
				{
					mouseWheelDelta = 0.0f;
				}
			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::GetInstance()->Uninit();

	CoUninitialize();

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	short delta = 0;  // 
	mouseWheelDelta = 0.0f;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;


	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_MOUSEWHEEL:

		delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouseWheelDelta = delta / (float)WHEEL_DELTA;  //最新のホイール値をセット（累積しない）
		lastWheelTime = GetTickCount64();  // 最後のスクロール時間を記録
		break;
	default:
		
		mouseWheelDelta = 0.0f;
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

