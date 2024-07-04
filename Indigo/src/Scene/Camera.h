#pragma once

class GraphicsDevice;

class Camera
{
public:

	float toRadians( float degrees ) {
		return degrees * (3.14159265359f / 180.0f);
	}

	float toDegrees( float radians ) {
		return radians * (180.0f / 3.14159265359f);
	}
	Camera (GraphicsDevice* graphicsDevice) : graphicsDevice( graphicsDevice ) {}

	void Initialize( int windowWidth, int windowHeight);
	void SetViewport();
	void changeAspectRatio( float newAspectRatio );
	void AdjustFOV( float fovChangeFactor );
	void Zoom( float zoomFactor );
	void Rotate( int dx, int dy );
	void MoveCamera( float dx, float dy, float dz );
	void stopMovement();
	void moveForward(  );
	void moveLeft(  );
	void moveBackward(  );
	void moveRight(  );
	void Update( double deltaTime );
	void calculateP();
	void calculateV();

	XMVECTOR getPosition() { return position; }
	XMMATRIX getW() { return worldMatrix; }
	XMMATRIX getV() { return viewMatrix; }
	XMMATRIX getP() { return projectionMatrix; }

private:
	GraphicsDevice* graphicsDevice = nullptr;

	const XMVECTOR WORLD_UP = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR position = XMVectorSet( 0.0f, 0.0f, -10.0f, 1.0f );  // Default position
	XMVECTOR forward = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 60.0f;
	const float FOV_CHANGE_SPEED = 3.0f;
	const float ZOOM_CHANGE_SPEED = .15f;
	const float ZOOM_MIN = 0.0f;
	const float ZOOM_MAX = 200.0f;
	const float ZOOM_DISTANCE_FACTOR = 0.4f;
	float aspectRatio = 0.0;
	float cameraDistance = 50.0f;

	float theta = -1.5f * XM_PI; // horizontal angle
	float phi = 1.8f;   // vertical angle
	const float epsilon = 0.0001f; // Small offset to prevent camera from reaching limits
	const float ROTATION_SPEED = 0.0005f; // Small offset to prevent camera from reaching limits

	XMVECTOR velocity = XMVectorSet( 0, 0, 0, 0 );
	XMVECTOR movementDirection = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	const float MOVE_SPEED = 20.0f;
	const float acceleration = 5.0f;
	const float dragFactor = 0.95f;
};