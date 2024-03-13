#include "pch.h"
#include "DirectXManager.h"


void DirectXManager::Initialize( HWND hWnd, int windowWidth, int windowHeight )
{
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &startCounter );
	QueryPerformanceCounter( &endCounter );

	mainDevice = new GraphicsDevice();
	mainDevice->Initialize( hWnd, windowWidth, windowHeight );
	
	// Load Shaders
	shaderManager = new ShaderManager(mainDevice, "assets/shaders/");
	std::shared_ptr<ShaderProgram> shaderProgramOpaque =			shaderManager->LoadShader( "BasicVS", "BasicPS" );
	std::shared_ptr<ShaderProgram> testshaderProgramOpaque =		shaderManager->LoadShader( "BasicVS", "testPS" );
	std::shared_ptr<ShaderProgram> shaderProgramOpaqueTextured =	shaderManager->LoadShader( "BasicTextureVS", "BasicTexturePS" );
	std::shared_ptr<ShaderProgram> shaderProgramTransparent =		shaderManager->LoadShader( "BasicVS", "TransparentPS" );
	std::shared_ptr<ShaderProgram> shaderProgramOpaqueWavey =		shaderManager->LoadShader( "WaveyVS", "BasicPS" );

	// Load RenderStates
	RenderState* renderStateOpaque = new RenderState( mainDevice );
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	renderStateOpaque->CreateRasterizerState( rasterDesc );
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	renderStateOpaque->CreateBlendState( blendDesc );
	renderStateOpaque->CreateBlendFactor( 1.0f, 1.0f, 1.0f, 1.0f );
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	renderStateOpaque->CreateDepthStencilState( dsDesc );
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderStateOpaque->CreateSamplerState( sampDesc );

	RenderState* renderStateTransparent = new RenderState( mainDevice );
	D3D11_RASTERIZER_DESC rasterDesc2{};
	rasterDesc2.CullMode = D3D11_CULL_NONE;
	rasterDesc2.FillMode = D3D11_FILL_SOLID;
	renderStateTransparent->CreateRasterizerState( rasterDesc2 );
	D3D11_BLEND_DESC blendDesc2{};
	blendDesc2.RenderTarget[0].BlendEnable = true;
	blendDesc2.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc2.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	renderStateTransparent->CreateBlendState( blendDesc2 );
	renderStateTransparent->CreateBlendFactor( 0.0f, 0.0f, 0.0f, 0.0f );
	D3D11_DEPTH_STENCIL_DESC dsDesc2 = {};
	dsDesc2.DepthEnable = true;
	dsDesc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // Disable depth writes
	dsDesc2.DepthFunc = D3D11_COMPARISON_LESS;             // Typical depth test
	renderStateTransparent->CreateDepthStencilState( dsDesc2 );

	Material* materialOpaque, * materialTransparent, * materialOpaqueWavey, * materialOpaqueTextured, * materialOpaqueTextured2;
	materialOpaque = new Material( mainDevice, shaderProgramOpaque, renderStateOpaque, false);
	materialOpaqueWavey = new Material( mainDevice, shaderProgramOpaqueWavey, renderStateOpaque, false );
	materialTransparent = new Material( mainDevice, shaderProgramTransparent, renderStateTransparent, true );
	materialOpaqueTextured = new Material( mainDevice, shaderProgramOpaqueTextured, renderStateOpaque, false );
	materialOpaqueTextured->LoadTexture( "cubePretty.glb" );
	materialOpaqueTextured2 = new Material( mainDevice, shaderProgramOpaqueTextured, renderStateOpaque, false );
	materialOpaqueTextured2->LoadTexture( "gun.glb" );


	Model* model;

	model = new Model( mainDevice );
	model->AddMeshEntry( new Mesh( mainDevice, "planeWrinkly.glb" ), materialOpaqueWavey );
	model->SetPosition( XMFLOAT3( 0.0f, -1.0f, 0.0f ) );
	model->SetScale( XMFLOAT3( 100.0f, 1.0f, 100.0f ) );
	AddModel( model );

	model = new Model( mainDevice );
	model->AddMeshEntry( new Mesh( mainDevice, "plane.glb" ), new Material( mainDevice, testshaderProgramOpaque, renderStateOpaque , false));
	model->SetPosition( XMFLOAT3( 0.0f, 25.0f, 50.0f ) );
	model->SetRotation( XMFLOAT3( -1.7f, 0.0f, 0.0f ) );
	model->SetScale( XMFLOAT3( 48.0f, 1.0f, 27.0f ) );
	AddModel( model );

	model = new Model( mainDevice );
	model->AddMeshEntry( new Mesh( mainDevice, "cube.glb" ), materialOpaque );
	model->SetPosition( XMFLOAT3( 3.0f, 0.0f, 0.0f ) );
	model->SetScale( XMFLOAT3( 1.0f, 1.0f, 0.2f ) );
	model->SetRotation( XMFLOAT3( 1.5f, 0.0f, 0.0f ) );
	AddModel( model );

	model = new Model( mainDevice );
	model->AddMeshEntry( new Mesh( mainDevice, "cubePretty.glb" ), materialOpaqueTextured );
	model->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	model->SetScale( XMFLOAT3( 100.0f, 100.0f, 100.0f ) );
	AddModel( model );

	model = new Model( mainDevice );
	model->AddMeshEntry( new Mesh( mainDevice, "gun.glb" ), materialOpaqueTextured2 );
	model->SetPosition( XMFLOAT3( 0.0f, 0.5f, 0.0f ) );
	model->SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
	//AddModel( model );

	model = new Model( mainDevice );
	model->AddMeshEntry( new Mesh( mainDevice, "cylinderPretty.glb" ), materialTransparent );
	AddModel( model );
	mainControllableModel = model;

	camera = new Camera(mainDevice);
	camera->Initialize( windowWidth, windowHeight );
}

void DirectXManager::Update()
{
	QueryPerformanceCounter( &startCounter );
	RenderFrame( totalTime );


	//if (elapsedTime >= 0.1) {
	//	UpdateGrid();
	//	elapsedTime -= 0.1; // Reset or decrement the elapsed time by the time span you checked for, so you can check again in the next intervals.
	//}

	QueryPerformanceCounter( &endCounter );
	frameTime = static_cast<double>(endCounter.QuadPart - startCounter.QuadPart) / frequency.QuadPart;
	//elapsedTime += frameTime;
	totalTime += frameTime; // Update the total time
#ifdef _DEBUG
	timeSinceLastShaderCheck += frameTime;
	if (timeSinceLastShaderCheck >= 0.3) {  // Check every second

		shaderManager->CheckForShaderUpdates();
		timeSinceLastShaderCheck = 0.0; 
	}
#endif
	camera->stopMovement();
	if (keyW) {
		camera->moveForward();
	}
	if (keyA) {
		camera->moveLeft();
	}
	if (keyS) {
		camera->moveBackward();
	}
	if (keyD) {
		camera->moveRight();
	}
	camera->Update( frameTime );
}

void DirectXManager::RenderFrame( double totalTime )
{
	// Clear the back buffer to a deep blue
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	mainDevice->GetDeviceContext()->ClearRenderTargetView( mainDevice->GetRenderTargetView(), color );
	mainDevice->GetDeviceContext()->ClearDepthStencilView( mainDevice->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

	std::vector<std::pair<Mesh*, Material*>> transparentMeshes;

	// First pass: Render opaque meshes and gather transparent meshes.
	for (auto& shaderPair : shaderMaterialToMeshesMap)
	{
		std::shared_ptr<ShaderProgram> currentShader = shaderPair.first;
		currentShader->Bind();

		for (auto& materialPair : shaderPair.second)
		{
			Material* currentMaterial = materialPair.first;
			currentMaterial->Setup();

			if (currentMaterial->isTransparent())
			{
				for (auto& mesh : materialPair.second)
				{
					transparentMeshes.push_back( { mesh, currentMaterial } );
				}
				continue; // Skip this iteration and handle transparent meshes later
			}

			for (auto& mesh : materialPair.second)
			{
				ConstantBuffer cb{};
				cb.Time = totalTime; // The time value you want to pass in
				cb.transformation = XMMatrixTranspose( mesh->GetParentModel()->GetTransformationMatrix() );
				cb.world = XMMatrixTranspose( camera->getW() ); // Transpose for HLSL
				cb.view = XMMatrixTranspose( camera->getV() );
				cb.projection = XMMatrixTranspose( camera->getP() );
				cb.ColorA = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
				cb.ColorB = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );

				currentMaterial->SetConstantBuffer( cb );
				//XMFLOAT3 a = mesh->GetParentModel()->GetPosition();
				//a.y = cos( (totalTime + a.x) * .2 ) * 10;
				//mesh->GetParentModel()->SetPosition( a );
				mesh->Draw();
			}
		}
	}

	// Sort the transparent meshes by distance from the camera (from farthest to closest).
	std::sort( transparentMeshes.begin(), transparentMeshes.end(),
		[&]( const std::pair<Mesh*, Material*>& a, const std::pair<Mesh*, Material*>& b ) -> bool {
			const XMFLOAT3& tempPosA = a.first->GetParentModel()->GetPosition();
			XMVECTOR posA = XMLoadFloat3( &tempPosA );

			const XMFLOAT3& tempPosB = b.first->GetParentModel()->GetPosition();
			XMVECTOR posB = XMLoadFloat3( &tempPosB );
			XMVECTOR camPosition = camera->getPosition();

			float distA = XMVectorGetX( XMVector3Length( camPosition - posA ) );
			float distB = XMVectorGetX( XMVector3Length( camPosition - posB ) );

			return distA > distB;
		} );


	// Render transparent meshes.
	for (auto& pair : transparentMeshes)
	{
		Mesh* mesh = pair.first;
		Material* currentMaterial = pair.second;
		currentMaterial->GetShaderProgram()->Bind();
		currentMaterial->Setup();

		ConstantBuffer cb{};
		cb.Time = totalTime; // The time value you want to pass in
		cb.transformation = XMMatrixTranspose( mesh->GetParentModel()->GetTransformationMatrix() );
		cb.world = XMMatrixTranspose( camera->getW() ); // Transpose for HLSL
		cb.view = XMMatrixTranspose( camera->getV() );
		cb.projection = XMMatrixTranspose( camera->getP() );
		cb.ColorA = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
		cb.ColorB = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );

		currentMaterial->SetConstantBuffer( cb );

		mesh->Draw();
	}
	mainDevice->GetSwapChain()->Present( 1, 0 );

	//ID2D1Factory1* d2dFactory = nullptr;
	//ID2D1Device* d2dDevice = nullptr;
	//ID2D1DeviceContext* d2dContext = nullptr;
	//ID2D1Bitmap1* d2dTargetBitmap = nullptr;
	//IDWriteFactory* dwFactory = nullptr;
	//IDWriteTextFormat* textFormat = nullptr;

	//D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory );
	//IDXGIDevice* dxgiDevice = nullptr;
	//mainDevice->GetDevice()->QueryInterface( &dxgiDevice );
	//d2dFactory->CreateDevice( dxgiDevice, &d2dDevice );
	//d2dDevice->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext );
	//IDXGISurface* dxgiBackBuffer = nullptr;
	//mainDevice->GetSwapChain()->GetBuffer( 0, __uuidof(IDXGISurface), (void**)&dxgiBackBuffer );
	//d2dContext->CreateBitmapFromDxgiSurface( dxgiBackBuffer, nullptr, &d2dTargetBitmap );
	//d2dContext->SetTarget( d2dTargetBitmap );

	//DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwFactory), reinterpret_cast<IUnknown**>(&dwFactory) );
	//dwFactory->CreateTextFormat( L"Arial", nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 32.0f, L"", &textFormat );

	//// Begin Direct2D drawing
	//d2dContext->BeginDraw();
	//d2dContext->Clear( D2D1::ColorF( D2D1::ColorF( 0.0f, 0.0f, 0.0f, 0.0f ) ) );
	//ID2D1SolidColorBrush* brush = nullptr;
	//d2dContext->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::White ), &brush );
	//d2dContext->DrawTextW( L"Hello DirectX", wcslen( L"Hello DirectX" ), textFormat, D2D1::RectF( 100, 100, 600, 600 ), brush );
	//d2dContext->EndDraw();

}

void DirectXManager::AddModel( Model* model ) {
	for (auto& entry : model->GetMeshEntries()) {
		Material* material = entry.material;
		std::shared_ptr<ShaderProgram> shader = material->GetShaderProgram();

		shaderMaterialToMeshesMap[shader][material].push_back( entry.mesh );
	}
}

void DirectXManager::handleLeftDown( WPARAM wParam, LPARAM lParam )
{

	if (!isMouseCaptured)
	{
		SetCapture( mainDevice->GetHWND() );
		ShowCursor( FALSE );  // Hide the cursor

		// Lock the mouse to the center of the window
		POINT center = { mainDevice->GetWindowWidth() / 2, mainDevice->GetWindowHeight() / 2 };
		ClientToScreen( mainDevice->GetHWND(), &center );
		SetCursorPos( center.x, center.y );

		lastMousePos.x = center.x;
		lastMousePos.y = center.y;

		isMouseCaptured = true;
	}
	
}

void DirectXManager::handleLeftUp( LPARAM lParam )
{

}

void DirectXManager::handleKeyDown( WPARAM wParam )
{

	switch (wParam)
	{
	case 'Q':
	{
		// If the model is currently scaled, reset it to original size.
		mainControllableModel->SetScale( XMFLOAT3( 1.0f, 1.0f, 1.0f ) );
		mainControllableModel->SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		mainControllableModel->SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
		static float x = 0.0f;
		static float z = 0.0f;
		break;
	}
	case 'W': keyW = true; break;
	case 'A': keyA = true; break;
	case 'S': keyS = true; break;
	case 'D': keyD = true; break;
	case 'R':
	{
		break;
	}
	case VK_ESCAPE:
	{
		ReleaseCapture();
		ShowCursor( TRUE );  // Show the cursor when window loses focus
		isMouseCaptured = false;
		break;
	}
	case VK_ADD:
	{
		float fovChangeFactor = 1.0f; // Convert to a zoom factor (-1.0 for down, 1.0 for up)
		camera->AdjustFOV( fovChangeFactor );
		break;
	}
	case VK_SUBTRACT:
	{
		float fovChangeFactor = -1.0f; // Convert to a zoom factor (-1.0 for down, 1.0 for up)
		camera->AdjustFOV( fovChangeFactor );
		break;
	}
	}
}

void DirectXManager::handleKeyUp( WPARAM wParam )
{
	switch (wParam) {
	case 'W': keyW = false; break;
	case 'A': keyA = false; break;
	case 'S': keyS = false; break;
	case 'D': keyD = false; break;
	}
}

void DirectXManager::handleMouseMove( WPARAM wParam, LPARAM lParam )
{
	if (isMouseCaptured)
	{
		POINT currentMousePos;
		GetCursorPos( &currentMousePos );

		int dx = currentMousePos.x - lastMousePos.x;
		int dy = currentMousePos.y - lastMousePos.y;

		camera->Rotate( dx, dy );

		// Re-center the cursor
		POINT center = { mainDevice->GetWindowWidth() / 2, mainDevice->GetWindowHeight() / 2 };

		ClientToScreen( mainDevice->GetHWND(), &center );
		SetCursorPos( center.x, center.y );

		lastMousePos.x = center.x;
		lastMousePos.y = center.y;

	}	
}

void DirectXManager::handleLostFocus(WPARAM wParam)
{
	if (LOWORD( wParam ) == WA_INACTIVE) // If the window is deactivated
	{
		if (isMouseCaptured)
		{
			ReleaseCapture();
			ShowCursor( TRUE );  // Show the cursor when window loses focus
			isMouseCaptured = false;
		}
	}

}

void DirectXManager::handleSetFocus()
{
#ifdef _DEBUG
	shaderManager->CheckForShaderUpdates();
	timeSinceLastShaderCheck = 0.0;
#endif
}

void DirectXManager::handleMouseWheel( WPARAM wParam )
{
	int wheelDelta = GET_WHEEL_DELTA_WPARAM( wParam ); // Get the amount the wheel was rotated
	float zoomFactor = -wheelDelta / (float)WHEEL_DELTA; // Convert to a zoom factor (-1.0 for down, 1.0 for up)
	camera->Zoom( zoomFactor );

}
void DirectXManager::handleSize( LPARAM lParam )
{
	// Obtain the new size
	int newWidth = LOWORD( lParam );
	int newHeight = HIWORD( lParam );

	// Resize back buffer, depth buffer, etc.
	mainDevice->ResizeBuffersAndViews( newWidth, newHeight );

	// Update aspect ratio and recreate projection matrix
	float aspectRatio = static_cast<float>(newWidth) / static_cast<float>(newHeight);

	camera->changeAspectRatio( aspectRatio );
}
