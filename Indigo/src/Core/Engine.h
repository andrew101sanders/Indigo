//#pragma once
//
//#include "Graphics/GraphicsDevice.h"
//#include "Graphics/ShaderManager.h"
//#include "Input/InputManager.h"
//#include "Scene/Camera.h"
//
//class Model;
//class Scene;
//
//class Engine
//{
//public:
//    Engine();
//    ~Engine();
//
//    void Initialize( HWND hWnd, int windowWidth, int windowHeight );
//    void Update();
//    void Render();
//
//    void HandleResize( int newWidth, int newHeight );
//    void HandleInput( UINT message, WPARAM wParam, LPARAM lParam );
//
//    // Add any other necessary methods
//
//private:
//    std::unique_ptr<GraphicsDevice> graphicsDevice;
//    std::unique_ptr<ShaderManager> shaderManager;
//    std::unique_ptr<InputManager> inputManager;
//    std::unique_ptr<Camera> camera;
//    std::unique_ptr<Scene> scene;
//
//    // Timing variables
//    LARGE_INTEGER frequency, startCounter, endCounter;
//    double totalTime;
//    double frameTime;
//
//    void InitializeScene();
//    void UpdateCamera( float deltaTime );
//};