// GameEngine.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "GameEngine.h"

#define CGLTF_IMPLEMENTATION
#define CGLTF_WRITE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4996)
#include "cgltf_write.h"
#include "cgltf.h"
#pragma warning(pop)

// Global Variables:
const int windowWidth = 1600;
const int windowHeight = 900;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    DirectXManager* dxManager = new DirectXManager();
    Win32Application winapp;
    winapp.Initialize(hInstance, nCmdShow, windowWidth, windowHeight);
    dxManager->Initialize(winapp.GetHwnd(), windowWidth, windowHeight);
    winapp.Run(dxManager);

}