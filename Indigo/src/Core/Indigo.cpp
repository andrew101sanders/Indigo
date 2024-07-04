// Indigo.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "Indigo.h"

#include "Core/Win32Application.h"
#include "Graphics/DirectXManager.h"

// Global Variables:
const int windowWidth = 1600;
const int windowHeight = 900;


int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow )
{
	DirectXManager* dxManager = new DirectXManager();
	Win32Application winapp;
	winapp.Initialize( hInstance, nCmdShow, windowWidth, windowHeight );
	dxManager->Initialize( winapp.GetHwnd(), windowWidth, windowHeight );
	winapp.Run( dxManager );
}