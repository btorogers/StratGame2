#pragma once

#include "Includes.h"
#include "Renderer.h"
#include "GameController.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

GameController* game;
InputController* input;

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
		NULL);

	ShowWindow(hWnd, nCmdShow);

	game = new GameController(hWnd);
	input = game->GetInputController();

	MSG msg;
	while (TRUE) {
		GetMessage(&msg, 0, 0, 0);
		//TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) {
			game->Quit();
			break;
		}
	}

	delete game;

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		} break;
		case WM_LBUTTONDOWN: {
			input->LeftMousePressed();
		} break;
		case WM_KEYDOWN: {
			input->KeyDown(wParam);
		} break;
		case WM_KEYUP: {
			input->KeyUp(wParam);
		} break;
		case WM_MOUSEWHEEL: {
			input->Scroll(wParam);
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}