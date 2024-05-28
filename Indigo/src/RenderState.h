#pragma once

#include "Indigo.h"

class RenderState
{
public:
	RenderState(GraphicsDevice* graphicsDevice) : graphicsDevice(graphicsDevice) {}
	~RenderState();

	void CreateRasterizerState(const D3D11_RASTERIZER_DESC& rasterDesc);
	void CreateBlendState(const D3D11_BLEND_DESC& blendDesc);
	void CreateBlendFactor(float, float, float, float);
	void CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& dsDesc);
	void CreateSamplerState( const D3D11_SAMPLER_DESC& sampDesc );

	void SetState();

private:
	ID3D11RasterizerState* rasterizerState = nullptr;
	ID3D11BlendState* blendState = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	float blendFactor[4] = { 1.f, 1.f, 1.f, 1.f};
	GraphicsDevice* graphicsDevice = nullptr;
};