#pragma once

#include "GameEngine.h"
#include "DirectXManager.h"

class Win32Application
{
public:
	static void Initialize(HINSTANCE hInstance, int nCmdShow, int windowWidth, int windowHeight);
	static int Run(DirectXManager* pSample);
	static HWND GetHwnd() { return hWnd; }
protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND hWnd;
};
