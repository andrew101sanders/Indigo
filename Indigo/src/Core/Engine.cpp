//// Engine.cpp
#include "pch.h"
//#include "Core/Engine.h"
//#include "Scene/Scene.h"
//#include "Scene/Model.h"
//
//Engine::Engine() : totalTime( 0 ), frameTime( 0 ) {}
//
//Engine::~Engine() = default;
//
//void Engine::Initialize( HWND hWnd, int windowWidth, int windowHeight )
//{
//    QueryPerformanceFrequency( &frequency );
//    QueryPerformanceCounter( &startCounter );
//
//    graphicsDevice = std::make_unique<GraphicsDevice>();
//    graphicsDevice->Initialize( hWnd, windowWidth, windowHeight );
//
//    shaderManager = std::make_unique<ShaderManager>( graphicsDevice.get(), "assets/shaders/" );
//    inputManager = std::make_unique<InputManager>();
//    camera = std::make_unique<Camera>( graphicsDevice.get() );
//    camera->Initialize( windowWidth, windowHeight );
//
//    scene = std::make_unique<Scene>();
//
//    InitializeScene();
//}
//
//void Engine::Update()
//{
//    QueryPerformanceCounter( &endCounter );
//    frameTime = static_cast<double>(endCounter.QuadPart - startCounter.QuadPart) / frequency.QuadPart;
//    totalTime += frameTime;
//    startCounter = endCounter;
//
//    inputManager->Update();
//    UpdateCamera( static_cast<float>(frameTime) );
//    scene->Update( frameTime );
//}
//
//void Engine::Render()
//{
//    graphicsDevice->ClearRenderTarget( { 0.0f, 0.2f, 0.4f, 1.0f } );
//    scene->Render( graphicsDevice.get(), camera.get(), totalTime );
//    graphicsDevice->Present();
//}
//
//void Engine::HandleResize( int newWidth, int newHeight )
//{
//    graphicsDevice->ResizeBuffersAndViews( newWidth, newHeight );
//    camera->changeAspectRatio( static_cast<float>(newWidth) / newHeight );
//}
//
//void Engine::HandleInput( UINT message, WPARAM wParam, LPARAM lParam )
//{
//    inputManager->ProcessInput( message, wParam, lParam );
//}
//
//void Engine::InitializeScene()
//{
//    // Initialize your scene here (similar to what was in DirectXManager::Initialize)
//    // Create models, materials, etc.
//}
//
//void Engine::UpdateCamera( float deltaTime )
//{
//    // Update camera based on input
//    // This logic was previously in DirectXManager::Update
//}