#include "pch.h"

#include "Graphics/GraphicsDevice.h"
#include "Graphics/ShaderProgram.h"

void ShaderProgram::LoadShader( LPCSTR VSFilename, LPCSTR PSFilename )
{
	// Load Shaders
    std::vector<byte> vsData = LoadShaderFile( VSFilename, VERTEX_SHADER );
    std::vector<byte> psData = LoadShaderFile( PSFilename, PIXEL_SHADER );

	// Assign vertex and pixel shaders
	HRESULT hr = graphicsDevice->GetDevice()->CreateVertexShader( vsData.data(), vsData.size(), nullptr, &vertexShader );
	assert( SUCCEEDED( hr ) );

	hr = graphicsDevice->GetDevice()->CreatePixelShader( psData.data(), psData.size(), NULL, &pixelShader );
	assert( SUCCEEDED( hr ) );

	// Assign input layout
	const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		  { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		  { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = graphicsDevice->GetDevice()->CreateInputLayout( inputElementDesc, ARRAYSIZE( inputElementDesc ), vsData.data(), vsData.size(), &inputLayout );
	assert( SUCCEEDED( hr ) );

	// ConstantBuffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( ConstantBuffer );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = graphicsDevice->GetDevice()->CreateBuffer( &bd, nullptr, &constantBuffer );
	assert( SUCCEEDED( hr ) );
}

void ShaderProgram::ClearResources()
{
    if (vertexShader)
    {
        vertexShader->Release();
        vertexShader = nullptr;
    }

    if (pixelShader)
    {
        pixelShader->Release();
        pixelShader = nullptr;
    }

    if (inputLayout)
    {
        inputLayout->Release();
        inputLayout = nullptr;
    }

    if (constantBuffer)
    {
        constantBuffer->Release();
        constantBuffer = nullptr;
    }
}


void ShaderProgram::UpdateBuffers( ConstantBuffer cb )
{
	graphicsDevice->GetDeviceContext()->UpdateSubresource( constantBuffer, 0, NULL, &cb, 0, 0 );
	graphicsDevice->GetDeviceContext()->VSSetConstantBuffers( 0, 1, &constantBuffer );
	graphicsDevice->GetDeviceContext()->PSSetConstantBuffers( 0, 1, &constantBuffer );
}

void ShaderProgram::Bind()
{
	graphicsDevice->GetDeviceContext()->VSSetShader( vertexShader, NULL, 0 );
	graphicsDevice->GetDeviceContext()->PSSetShader( pixelShader, NULL, 0 );
	graphicsDevice->GetDeviceContext()->IASetInputLayout( inputLayout ); 
}

bool ShaderProgram::CompileHLSL( const std::string& filenameWithoutExtension, ShaderType shaderType )
{
    LPCSTR entryPoint;
    LPCSTR shaderModel;

    switch (shaderType)
    {
    case VERTEX_SHADER:
        entryPoint = "VSmain";
        shaderModel = "vs_5_0";
        break;
    case PIXEL_SHADER:
        entryPoint = "PSmain";
        shaderModel = "ps_5_0";
        break;
    default:
        return false;
    }

    // Construct the full path of the HLSL file.
    std::string fullHLSLPath = "assets/shaders/" + filenameWithoutExtension + ".hlsl";
    std::ifstream hlslFile( fullHLSLPath, std::ios::binary | std::ios::ate );

    if (!hlslFile.is_open())
    {
        return false;
    }

    const size_t fileSize = static_cast<size_t>(hlslFile.tellg());
    hlslFile.seekg( 0, std::ios::beg );

    std::vector<char> hlslData( fileSize );
    if (!hlslFile.read( hlslData.data(), fileSize ))
    {
        return false;
    }

    hlslFile.close();

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompile(
        hlslData.data(),
        hlslData.size(),
        nullptr,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        shaderModel,
        0,
        0,
        &shaderBlob,
        &errorBlob
    );

    if (FAILED( hr ))
    {
        if (errorBlob)
        {
            char* errorMsg = static_cast<char*>(errorBlob->GetBufferPointer());
            // Handle the error, possibly log the error message.
            errorBlob->Release();
        }
        return false;
    }

    // Construct the path for the output .cso file.
    std::string outputPath = "assets/shaders/shadercache/" + filenameWithoutExtension + ".cso";

    std::ofstream csoFile( outputPath, std::ios::binary );
    csoFile.write( static_cast<char*>(shaderBlob->GetBufferPointer()), shaderBlob->GetBufferSize() );
    csoFile.close();

    shaderBlob->Release();

    return true;
}

std::vector<byte> ShaderProgram::LoadShaderFile( const std::string& filenameWithoutExtension, ShaderType shaderType )
{
    // Construct the full path for the CSO file.
    std::string csoPath = "assets/shaders/shadercache/" + filenameWithoutExtension + ".cso";
    std::ifstream file( csoPath, std::ios::binary | std::ios::ate );

    // If the CSO file isn't found.
    if (!file.is_open())
    {
        if (!CompileHLSL( filenameWithoutExtension, shaderType ))
        {
            switch (shaderType)
            {
            case PIXEL_SHADER:
                file.open( "assets/shaders/shadercache/ErrorPS.cso", std::ios::binary | std::ios::ate );
                break;
            case VERTEX_SHADER:
                file.open( "assets/shaders/shadercache/ErrorVS.cso", std::ios::binary | std::ios::ate );
                break;
            }
            
            if (!file.is_open())
            {
                throw std::runtime_error( "Failed to open fallback CSO file" );
            }
        }
        else
        {
            // If the HLSL compilation was successful, you might have a new .cso file.
            // Try to open it again.
            file.open( csoPath, std::ios::binary | std::ios::ate );
            if (!file.is_open())
            {
                throw std::runtime_error( "Failed to open CSO file after compilation: " + csoPath );
            }
        }
    }

    const size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg( 0, std::ios::beg );

    std::vector<byte> fileData( fileSize );
    if (!file.read( reinterpret_cast<char*>(fileData.data()), fileSize ))
    {
        throw std::runtime_error( "Failed to read CSO file: " + filenameWithoutExtension );
    }

    file.close();
    return fileData;
}

