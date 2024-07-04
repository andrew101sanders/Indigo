#pragma once

class GraphicsDevice
{
public:

	void Initialize(HWND hWnd, int width, int height);
	void ResizeBuffersAndViews(int width, int height);
	ID3D11Device* GetDevice() const { return device; }
	ID3D11DeviceContext* GetDeviceContext() const { return deviceContext; }
	IDXGISwapChain* GetSwapChain() const { return swapChain; }
	ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView; }
	int GetWindowWidth() { return windowWidth; }
	int GetWindowHeight() { return windowHeight; }
	HWND GetHWND() { return hWnd; };
private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;

	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;

	int windowWidth;
	int windowHeight;
	HWND hWnd;

};