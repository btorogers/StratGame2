#include "Includes.h"
#include "Renderer.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Renderer *r;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	WNDCLASSEX wc;

	srand(GetTickCount());

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass1";

	RegisterClassEx(&wc);

	RECT wr = {0, 0, Renderer::SCREEN_WIDTH, Renderer::SCREEN_HEIGHT};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"WindowClass1",
		"Hi there",
		WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	r = new Renderer(hWnd);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;
	DWORD timePrev = GetTickCount();
	DWORD timeCurrent = GetTickCount();

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		}
		timeCurrent = GetTickCount();
		if (timeCurrent > timePrev + 5) {
			timePrev = timeCurrent;
			r->RenderFrame();
		}
	}

	delete r;
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// sort through and find what code to run for the message given
	switch (message) {
		case WM_DESTROY: {
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_KEYDOWN: {
			r->AddRandomCuboid();
		} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}