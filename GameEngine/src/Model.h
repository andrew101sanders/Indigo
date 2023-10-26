#pragma once

#include "GameEngine.h"
#include "GraphicsDevice.h"
#include "Material.h"

class Mesh;

class Model {
public:
	struct MeshEntry {
		Mesh* mesh;
		Material* material;
	};

	Model( GraphicsDevice* graphicsDevice ) : graphicsDevice( graphicsDevice ) {}

	void AddMeshEntry( Mesh* mesh, Material* material );
	void SetPosition( const DirectX::XMFLOAT3& pos );
	void SetRotation( const DirectX::XMFLOAT3& rot );
	void SetScale( const DirectX::XMFLOAT3& scl );
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation( ) { return rotation; }
	DirectX::XMFLOAT3 GetScale( ) { return scale; }
	void UpdateTransformationMatrix();
	XMMATRIX GetTransformationMatrix();
	// Functions to draw the model, set materials, etc.
	std::vector<MeshEntry> GetMeshEntries() { return meshEntries; }

private:

	std::vector<MeshEntry> meshEntries;

	DirectX::XMFLOAT3 position{ 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation{ 0.0f, 0.0f, 0.0f }; // Represented as Euler angles in this example
	DirectX::XMFLOAT3 scale{ 1.0f, 1.0f, 1.0f };

	DirectX::XMMATRIX transformationMatrix = XMMatrixIdentity();

	GraphicsDevice* graphicsDevice;
};