#include "pch.h"
#include "Win32Application.h"

HWND Win32Application::hWnd = nullptr;

void Win32Application::Initialize( HINSTANCE hInstance, int nCmdShow, int windowWidth, int windowHeight )
{
	WNDCLASSEXW wcex{};
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_GAMEOFLIFEDIRECT3D ) );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = L"GAMEOFLIFEDIRECT3D";
	RegisterClassExW( &wcex );

	RECT windowRect = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
	AdjustWindowRect( &windowRect, WS_OVERLAPPEDWINDOW, FALSE );

	// Calculate centered window position
	int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYSCREEN );
	int windowPosX = (screenWidth - (windowRect.right - windowRect.left)) / 2;
	int windowPosY = (screenHeight - (windowRect.bottom - windowRect.top)) / 2;

	hWnd = CreateWindowW( L"GAMEOFLIFEDIRECT3D", L"GameOfLifeDirect3D", WS_OVERLAPPEDWINDOW,
		windowPosX, windowPosY, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr );

	// Set the window to be topmost to overlap the taskbar
	SetWindowPos( hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
}


int Win32Application::Run( DirectXManager* dXManager )
{

	SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dXManager) );

	// Main message/game loop:
	MSG msg;
	while (true)
	{
		if (PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			if (msg.message == WM_QUIT) break;
		}
		else
		{
			dXManager->Update();
		}
	}
	return (int)msg.wParam;
}

LRESULT Win32Application::WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	DirectXManager* dXManager = reinterpret_cast<DirectXManager*>(GetWindowLongPtr( hWnd, GWLP_USERDATA ));

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		if (dXManager)
		{
			dXManager->handleLeftDown( wParam, lParam );
		}
		return 0;
		break;
	}
	case WM_LBUTTONUP:
	{
		if (dXManager)
		{
			dXManager->handleLeftUp( lParam );
		}
		return 0;
		break;
	}
	case WM_KEYDOWN:
	{
		if (dXManager)
		{
			dXManager->handleKeyDown( wParam );
		}
		return 0;
		break;
	}
	case WM_KEYUP:
	{
		if (dXManager)
		{
			dXManager->handleKeyUp( wParam );
		}
		return 0;
		break;
	}
	case WM_MOUSEMOVE:
	{

		if (dXManager)
		{
			dXManager->handleMouseMove( wParam, lParam );
		}

		return 0;
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (dXManager)
		{
			dXManager->handleMouseWheel( wParam );
		}
		return 0;
		break;
	}
	case WM_ACTIVATE:
	{
		if (dXManager)
		{
			dXManager->handleLostFocus( wParam );
		}

		return 0;
		break;
	}
	case WM_SETFOCUS:
		if (dXManager)
		{
			dXManager->handleSetFocus();
		}
		return 0;
		break;
	case WM_SIZE:
	{
		if (dXManager)
		{
			dXManager->handleSize( lParam );
		}
		return 0;
		break;
	}
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
}
