#include "pch.h"

#include "Graphics/GraphicsDevice.h"
#include "Graphics/RenderState.h"

RenderState::~RenderState()
{
	if (rasterizerState) rasterizerState->Release();
	if (blendState) blendState->Release();
	if (depthStencilState) depthStencilState->Release();
}

void RenderState::CreateRasterizerState( const D3D11_RASTERIZER_DESC& rasterDesc )
{
	HRESULT hr = graphicsDevice->GetDevice()->CreateRasterizerState( &rasterDesc, &rasterizerState );
	if (FAILED( hr )) {
		assert( false );
	}
}

void RenderState::CreateBlendState( const D3D11_BLEND_DESC& blendDesc )
{
	HRESULT hr = graphicsDevice->GetDevice()->CreateBlendState( &blendDesc, &blendState );
	if (FAILED( hr )) {
		assert( false );
	}
}

void RenderState::CreateBlendFactor( float f1, float f2, float f3, float f4 )
{
	blendFactor[0] = f1;
	blendFactor[1] = f2;
	blendFactor[2] = f3;
	blendFactor[3] = f4;
}

void RenderState::CreateDepthStencilState( const D3D11_DEPTH_STENCIL_DESC& dsDesc )
{
	HRESULT hr = graphicsDevice->GetDevice()->CreateDepthStencilState( &dsDesc, &depthStencilState );
	if (FAILED( hr )) {
		assert( false );
	}
}

void RenderState::CreateSamplerState( const D3D11_SAMPLER_DESC& sampDesc )
{
	HRESULT hr = graphicsDevice->GetDevice()->CreateSamplerState( &sampDesc, &samplerState );
	if (FAILED( hr )) {
		assert( false );
	}

}

void RenderState::SetState()
{

	graphicsDevice->GetDeviceContext()->PSSetSamplers( 0, 1, &samplerState );

	graphicsDevice->GetDeviceContext()->RSSetState( rasterizerState );
	graphicsDevice->GetDeviceContext()->OMSetBlendState( blendState, blendFactor, 0xffffffff );
	graphicsDevice->GetDeviceContext()->OMSetDepthStencilState( depthStencilState, 0 );
}