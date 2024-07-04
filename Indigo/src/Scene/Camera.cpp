#include "pch.h"

#include "Graphics/GraphicsDevice.h"

#include "Scene/Camera.h"

void Camera::Initialize( int windowWidth, int windowHeight)
{
	SetViewport();
	aspectRatio = (float)windowWidth / (float)windowHeight;

	// Initialize and set View, Projection, and World Matrices
	calculateV();
	calculateP();
	worldMatrix = XMMatrixIdentity();
}

void Camera::SetViewport()
{
	// Set Viewport
	RECT winRect;
	GetClientRect( graphicsDevice->GetHWND(), &winRect );
	D3D11_VIEWPORT viewport = {
		0.0f,
		0.0f,
		(FLOAT)(winRect.right - winRect.left),
		(FLOAT)(winRect.bottom - winRect.top),
		0.0f,
		1.0f };
	graphicsDevice->GetDeviceContext()->RSSetViewports( 1, &viewport );
}

void Camera::changeAspectRatio( float newAspectRatio )
{
	aspectRatio = newAspectRatio;
	calculateP();
}

void Camera::AdjustFOV( float fovChangeFactor )
{
	// Convert FOV to radians
	float fovInRadians = toRadians( fov );

	// Adjust the FOV based on the zoom factor
	fovInRadians += fovChangeFactor * toRadians( FOV_CHANGE_SPEED );

	// Optional: Clamp the FOV to prevent it from becoming too narrow or too wide
	fovInRadians = max( toRadians( 0.1f ), min( toRadians( 120.0f ), fovInRadians ) );

	// Convert the clamped FOV back to degrees
	fov = toDegrees( fovInRadians );

	calculateP();
}

void Camera::Zoom( float zoomFactor )
{
	float speedMultiplier = 1.0f + std::fabs( cameraDistance ) * ZOOM_DISTANCE_FACTOR;
	cameraDistance = std::clamp( cameraDistance + zoomFactor * ZOOM_CHANGE_SPEED * speedMultiplier, ZOOM_MIN, ZOOM_MAX );
}


void Camera::Rotate( int dx, int dy )
{

	theta -= dx * ROTATION_SPEED;

	// Wrap theta around [0, 2*pi]
	theta = fmodf( theta, XM_2PI );
	if (theta < 0.0f) {  // handle negative values
		theta += XM_2PI;
	}

	phi += dy * ROTATION_SPEED;
	phi = max( epsilon, min( XM_PI - epsilon, phi ) );
}

void Camera::MoveCamera( float dx, float dy, float dz )
{
	// Calculate the forward, right, and up vectors
	float x = sinf( phi ) * cosf( theta );
	float y = cosf( phi );
	float z = sinf( phi ) * sinf( theta );

	XMVECTOR forward = XMVectorSet( x, y, z, 0.0f );
	XMVECTOR right = XMVector3Cross( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), forward ); // Right vector using world up
	XMVECTOR up = XMVector3Cross( forward, right ); // Up vector

	// Scale the movement based on the desired dx, dy, and dz
	position += forward * dz;
	position += right * dx;
	position += up * dy;

	calculateV(); // Recalculate the view matrix with the updated position
}

void Camera::stopMovement()
{
	movementDirection = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
}

void Camera::moveForward()
{
	float sinPhi = sinf( phi );
	float x = sinPhi * cosf( theta );
	float y = 0.0f; // cosf( phi );
	float z = sinPhi * sinf( theta );
	XMVECTOR forward = XMVectorSet( x, y, z, 0.0f );
	movementDirection += forward;
}

void Camera::moveLeft()
{
	float sinPhi = sinf( phi );
	float x = sinPhi * cosf( theta );
	float y = 0.0f; // cosf( phi );
	float z = sinPhi * sinf( theta );
	XMVECTOR forward = XMVectorSet( x, y, z, 0.0f );
	XMVECTOR right = XMVector3Cross( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), forward );
	movementDirection -= right;
}

void Camera::moveBackward(  )
{
	float sinPhi = sinf( phi );
	float x = sinPhi * cosf( theta );
	float y = 0.0f; // cosf( phi );
	float z = sinPhi * sinf( theta );
	XMVECTOR forward = XMVectorSet( x, y, z, 0.0f );
	movementDirection -= forward;
}

void Camera::moveRight(  )
{
	float sinPhi = sinf( phi );
	float x = sinPhi * cosf( theta );
	float y = 0.0f; // cosf( phi );
	float z = sinPhi * sinf( theta );
	XMVECTOR forward = XMVectorSet( x, y, z, 0.0f );
	XMVECTOR right = XMVector3Cross( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), forward );
	movementDirection += right;
}

void Camera::Update( double deltaTime )
{
	float fdeltaTime = static_cast<float>(deltaTime);
	movementDirection = XMVector3Normalize( movementDirection );

	// Compute the target velocity based on the desired direction of movement.
	XMVECTOR targetVelocity = movementDirection * MOVE_SPEED;

	// Accelerate towards the target velocity
	XMVECTOR accelerationVector = (targetVelocity - velocity) * acceleration;
	velocity += accelerationVector * fdeltaTime;

	// Apply drag to the velocity
	velocity *= powf( dragFactor, fdeltaTime );

	// Update camera position based on velocity
	position += velocity * fdeltaTime;

	calculateV();  // Update view matrix with new position.
}


void Camera::calculateP()
{
	// Recalculate the projection matrix with the new FOV
	projectionMatrix = XMMatrixPerspectiveFovLH( toRadians( fov ), aspectRatio, nearPlane, farPlane );
}

void Camera::calculateV()
{
	// Calculate direction based on angles
	XMVECTOR direction = XMVectorSet(
		sinf( phi ) * cosf( theta ),
		cosf( phi ),
		sinf( phi ) * sinf( theta ),
		0.0f
	);

	// Determine Eye and At positions based on distance
	XMVECTOR Eye = position - direction * cameraDistance;
	XMVECTOR At = position + direction;  // Always look forward

	// Compute the view matrix
	viewMatrix = XMMatrixLookAtLH( Eye, At, WORLD_UP );
}

