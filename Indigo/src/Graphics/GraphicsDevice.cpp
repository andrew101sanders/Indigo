#include "pch.h"
#include "Graphics/GraphicsDevice.h"

void GraphicsDevice::Initialize(HWND hWnd, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    this->hWnd = hWnd;

    // Init Swapchain
    DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
    swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
    swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_descr.SampleDesc.Count = 1;
    swap_chain_descr.SampleDesc.Quality = 0;
    swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_descr.BufferCount = 1;
    swap_chain_descr.OutputWindow = hWnd;
    swap_chain_descr.Windowed = true;

    // Create device, deviceContext, and swap chain
    D3D_FEATURE_LEVEL feature_level;
    UINT flags = D3D11_CREATE_DEVICE_DEBUG;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swap_chain_descr,
        &swapChain,
        &device,
        &feature_level,
        &deviceContext);
    assert(S_OK == hr && swapChain && device && deviceContext);

    // Setup render target
    ID3D11Texture2D* framebuffer{};
    hr = swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (LPVOID*)&framebuffer);
    assert(SUCCEEDED(hr));
    hr = device->CreateRenderTargetView(
        framebuffer, 0, &renderTargetView);
    assert(SUCCEEDED(hr));
    D3D11_TEXTURE2D_DESC backBufferDesc;
    framebuffer->GetDesc(&backBufferDesc);
    framebuffer->Release();

    // DepthStencilBuffer and DepthStencilView Initialization
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    depthBufferDesc.Width = windowWidth;  // Assuming you have width and height defined, adjust accordingly.
    depthBufferDesc.Height = windowHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
    assert(SUCCEEDED(hr));

    hr = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
    assert(SUCCEEDED(hr));

    deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void GraphicsDevice::ResizeBuffersAndViews(int width, int height)
{
    // Check for minimized window
    if (width == 0 || height == 0)
        return;

    windowWidth = width;
    windowHeight = height;
    if (deviceContext && renderTargetView && depthStencilView)
    {
        // Release existing render target view
        renderTargetView->Release();

        // Release the old depth stencil view & buffer
        depthStencilView->Release();
        depthStencilBuffer->Release();

        // Resize the swap chain's back buffer
        swapChain->ResizeBuffers(0, windowWidth, windowHeight, DXGI_FORMAT_UNKNOWN, 0);

        // Create a new render target view for the resized back buffer
        ID3D11Texture2D* backBuffer = nullptr;
        HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (SUCCEEDED(hr) && backBuffer)
        {
            hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
            assert(SUCCEEDED(hr)); // or handle the error appropriately
            backBuffer->Release();
        }
        else
        {
            // Handle the error (logging, throwing an exception, etc.)
            assert(false); // or your preferred error handling mechanism
        }

        // Set Viewport
        RECT winRect;
        GetClientRect(hWnd, &winRect);
        D3D11_VIEWPORT viewport = {
            0.0f,
            0.0f,
            (FLOAT)(winRect.right - winRect.left),
            (FLOAT)(winRect.bottom - winRect.top),
            0.0f,
            1.0f };
        deviceContext->RSSetViewports(1, &viewport);

        D3D11_TEXTURE2D_DESC depthBufferDesc = {};
        depthBufferDesc.Width = windowWidth;
        depthBufferDesc.Height = windowHeight;
        depthBufferDesc.MipLevels = 1;
        depthBufferDesc.ArraySize = 1;
        depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthBufferDesc.SampleDesc.Count = 1;
        depthBufferDesc.SampleDesc.Quality = 0;
        depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthBufferDesc.CPUAccessFlags = 0;
        depthBufferDesc.MiscFlags = 0;

        hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
        assert(SUCCEEDED(hr));

        hr = device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
        assert(SUCCEEDED(hr));

        deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    }
}
