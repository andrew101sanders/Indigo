#pragma once

class ShaderProgram;
class RenderState;
class GraphicsDevice;
struct ConstantBuffer;

class Material
{
public:
	Material(GraphicsDevice* graphicsDevice, std::shared_ptr<ShaderProgram> shader, RenderState* renderState, bool transparent) :
		graphicsDevice(graphicsDevice),
		shader(shader),
		renderState(renderState),
		transparent(transparent) {}
	void LoadTexture( LPCSTR filename );
	void SetConstantBuffer(ConstantBuffer cb);
	void Setup( );
	bool isTransparent() { return transparent; }

	std::shared_ptr<ShaderProgram> GetShaderProgram() const {	return shader;}
	RenderState* GetRenderState() const {	return renderState;}
private:
	std::shared_ptr<ShaderProgram> shader = nullptr;
	RenderState* renderState = nullptr;
	GraphicsDevice* graphicsDevice = nullptr;
	ID3D11ShaderResourceView* textureSRV = nullptr;
	bool transparent = false;
};