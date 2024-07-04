#pragma once

struct ConstantBuffer
{
	XMMATRIX transformation;
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
    XMFLOAT4 ColorA;
    XMFLOAT4 ColorB;
	double Time;
	float padding[2];
};

enum ShaderType
{
	VERTEX_SHADER,
	PIXEL_SHADER
};

class ShaderProgram
{
public:
	ShaderProgram(GraphicsDevice* graphicsDevice) : graphicsDevice(graphicsDevice) {}
	void LoadShader(LPCSTR VSFilename, LPCSTR PSFilename);
	void ClearResources();
	void UpdateBuffers(ConstantBuffer cb);
	void Bind();

private:
	bool CompileHLSL( const std::string& filename, ShaderType shaderType );
	std::vector<byte> LoadShaderFile( const std::string& filename, ShaderType shaderType );
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	GraphicsDevice* graphicsDevice = nullptr;
	ID3D11Buffer* constantBuffer = nullptr;
};