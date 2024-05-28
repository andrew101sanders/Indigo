#pragma once

#include "Indigo.h"
#include "GraphicsDevice.h"

class Model;

struct VertexType
{
	XMFLOAT4 position;
	XMFLOAT2 texCoords; //uv
	XMFLOAT3 normal;
};

class Mesh
{
public:
	Mesh( GraphicsDevice* graphicsDevice, LPCSTR filename );
	void SetParentModel( Model* model ) { parentModel = model; }

	Model* GetParentModel() const { return parentModel; }
	void LoadMesh( LPCSTR );
	void Draw();
private:
	Model* parentModel = nullptr;
	std::vector<VertexType> vertices;
	std::vector<unsigned int> indices;

	GraphicsDevice* graphicsDevice = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
};