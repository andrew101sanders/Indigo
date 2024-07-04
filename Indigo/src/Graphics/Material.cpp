#include "pch.h"

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Material.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/RenderState.h"

void Material::LoadTexture( LPCSTR filenameIn ) {
    std::string baseDir = "assets/meshes/";
    std::string filename = baseDir + filenameIn;

    // Load the glTF file using cgltf
    cgltf_options options = {};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file( &options, filename.c_str(), &data );

    if (result != cgltf_result_success) {
        assert( false );
        return;
    }
    result = cgltf_load_buffers( &options, data, filename.c_str() );
    if (result != cgltf_result_success) {
        assert( false );
        cgltf_free( data );
        return;
    }

    if (data->textures_count == 0) {
        assert( false ); // No texture found in the glTF file
        cgltf_free( data );
        return;
    }

    cgltf_texture* tex = &data->textures[0]; // Assume we want the first texture
    cgltf_image* img = tex->image;

    std::vector<unsigned char> decodedImage;
    unsigned width, height;

    if (img->buffer_view) { // Texture is embedded

        if (!img->buffer_view->buffer->data) {
            assert( false ); // Buffer data not loaded
            return;
        }

        size_t size = img->buffer_view->size;
        const unsigned char* buffer = static_cast<const unsigned char*>(img->buffer_view->buffer->data) + img->buffer_view->offset;

        unsigned error = lodepng::decode( decodedImage, width, height, buffer, size, LCT_RGBA, 8 );

        if (error) {
            assert( false ); // PNG decoding failed
            cgltf_free( data );
            return;
        }
    }
    else if (img->uri) {
        // Here you would load the texture from the URI (it might be a file path, or a data URI)
        assert( false ); // URI-based textures are not handled in this example
        cgltf_free( data );
        return;
    }
    else {
        assert( false ); // No image data found
        cgltf_free( data );
        return;
    }

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = decodedImage.data();
    initData.SysMemPitch = width * 4; // 4 bytes per pixel for RGBA

    ID3D11Texture2D* texture2D = nullptr;
    HRESULT hr = graphicsDevice->GetDevice()->CreateTexture2D( &texDesc, &initData, &texture2D );

    if (FAILED( hr )) {
        assert( false );
        return;
    }

    hr = graphicsDevice->GetDevice()->CreateShaderResourceView( texture2D, nullptr, &textureSRV );
    if (FAILED( hr )) {
        assert( false );
        return;
    }

    texture2D->Release();

    // Free the cgltf data when done
    cgltf_free( data );
}

void Material::SetConstantBuffer( ConstantBuffer cb )
{

    shader->UpdateBuffers( cb );

}

void Material::Setup()
{

    graphicsDevice->GetDeviceContext()->PSSetShaderResources( 0, 1, &textureSRV );

    renderState->SetState();
}

