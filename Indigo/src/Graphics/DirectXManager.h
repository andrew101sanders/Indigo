#pragma once

class Model;
class Mesh;
class Material;
class GraphicsDevice;
class ShaderProgram;
class ShaderManager;
class Camera;

class DirectXManager
{
public:
	void Initialize(HWND hWnd, int windowWidth, int windowHeight);
	void Update();
	void RenderFrame(double totalTime);
	void AddModel( Model* model );

	void handleLeftDown( WPARAM, LPARAM);
	void handleLeftUp(LPARAM);
	void handleKeyDown(WPARAM);
	void handleKeyUp( WPARAM );
	void handleMouseMove(WPARAM, LPARAM);
	void handleLostFocus( WPARAM wParam );
	void handleSetFocus();
	void handleMouseWheel(WPARAM);
	void handleSize(LPARAM);
private:
	GraphicsDevice *mainDevice;
	ShaderManager* shaderManager;
	std::unordered_map<std::shared_ptr<ShaderProgram>, std::unordered_map<Material*, std::vector<Mesh*>>> shaderMaterialToMeshesMap;

#ifdef _DEBUG
	double timeSinceLastShaderCheck = 0.0;
#endif
	Model* mainControllableModel = nullptr;

	LARGE_INTEGER frequency, startCounter, endCounter;
	double targetFrameRate = 60.0; // 60 FPS
	double targetFrameTime = 1.0 / targetFrameRate; // Approx. 0.01667 seconds per frame
	double elapsedTime = 0.0;
	double totalTime = 0.0;
	double frameTime = 0.0;
	Camera* camera;
	POINT lastMousePos; // Stores last known mouse position
	bool isMouseCaptured = false;

	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;
};