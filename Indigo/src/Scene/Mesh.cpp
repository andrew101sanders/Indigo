#include "pch.h"

#include "Graphics/GraphicsDevice.h"

#include "Scene/Model.h"
#include "Scene/Mesh.h"

Mesh::Mesh(GraphicsDevice* graphicsDevice, LPCSTR filename)
{
    this->graphicsDevice = graphicsDevice;
    LoadMesh(filename);
}

void Mesh::LoadMesh( LPCSTR filenameIn ) {
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

    if (data->meshes_count == 0) {
        assert( false ); // No mesh found in the glTF file
        cgltf_free( data );
        return;
    }

    vertices.clear();
    indices.clear();

    for (int meshid = 0; meshid < data->meshes_count; ++meshid)
    {
        cgltf_mesh* mesh = &data->meshes[meshid]; // Assume we want the first mesh
        cgltf_primitive* primitive = &mesh->primitives[0]; // Assume we want the first primitive

        if (!primitive->attributes_count || !primitive->indices) {
            assert( false ); // Primitive doesn't have vertices or indices
            cgltf_free( data );
            return;
        }

        // Load vertex data
        for (size_t i = 0; i < primitive->attributes_count; i++) {
            cgltf_attribute* attribute = &primitive->attributes[i];
            cgltf_accessor* accessor = attribute->data;
            cgltf_buffer_view* bufferView = accessor->buffer_view;
            const char* bufferData = (char*)bufferView->buffer->data + bufferView->offset;

            if (attribute->type == cgltf_attribute_type_position) {
                for (size_t j = 0; j < accessor->count; j++) {
                    float* vertex_data = (float*)(bufferData + accessor->stride * j);
                    VertexType vertex{};
                    vertex.position = DirectX::XMFLOAT4( vertex_data[0], vertex_data[1], vertex_data[2], 1 );
                    vertices.push_back( vertex );
                }
            }
            else if (attribute->type == cgltf_attribute_type_texcoord) {
                for (size_t j = 0; j < accessor->count; j++) {
                    float* texcoord_data = (float*)(bufferData + accessor->stride * j);
                    vertices[j].texCoords = DirectX::XMFLOAT2( texcoord_data[0], texcoord_data[1] );
                }
            }
            else if (attribute->type == cgltf_attribute_type_normal) {
                for (size_t j = 0; j < accessor->count; j++) {
                    float* normal_data = (float*)(bufferData + accessor->stride * j);
                    vertices[j].normal = DirectX::XMFLOAT3( normal_data[0], normal_data[1], normal_data[2] );
                }
            }
            // Add more cases if you need other attributes.
        }

        // Load index data
        cgltf_accessor* indexAccessor = primitive->indices;
        cgltf_buffer_view* indexBufferView = indexAccessor->buffer_view;
        const char* indexBufferData = (char*)indexBufferView->buffer->data + indexBufferView->offset;

        for (size_t i = 0; i < indexAccessor->count; i++) {
            unsigned int index;
            if (indexAccessor->component_type == cgltf_component_type_r_16u) {
                index = *(unsigned short*)(indexBufferData + indexAccessor->stride * i);
            }
            else if (indexAccessor->component_type == cgltf_component_type_r_32u) {
                index = *(unsigned int*)(indexBufferData + indexAccessor->stride * i);
            }
            else {
                assert( false );  // Unsupported index type or handle other cases
            }
            indices.push_back( index );

        }
    }
    
    size_t bufferSize = sizeof(VertexType) * vertices.size();
    assert(bufferSize <= UINT_MAX);  // Ensure the buffer size doesn't exceed the maximum value for UINT

    // Create and fill vertex buffer
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = static_cast<UINT>(bufferSize);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();

    HRESULT hr = graphicsDevice->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    assert(SUCCEEDED(hr));

    size_t indexBufferSize = sizeof(unsigned int) * indices.size();
    assert(indexBufferSize <= UINT_MAX);  // Ensure the buffer size doesn't exceed the maximum value for UINT

    // Create and fill index buffer
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = static_cast<UINT>(indexBufferSize);
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();

    hr = graphicsDevice->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    assert(SUCCEEDED(hr));
    cgltf_free( data );

}

void Mesh::Draw()
{
    graphicsDevice->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set Input Assembler
    UINT vertexStride = sizeof(VertexType);
    UINT vertexOffset = 0;
    graphicsDevice->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);
    graphicsDevice->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    size_t numIndices = indices.size();
    assert(numIndices <= UINT_MAX);  // Ensure the number of indices doesn't exceed the maximum value for UINT
    graphicsDevice->GetDeviceContext()->DrawIndexed(static_cast<UINT>(numIndices), 0, 0);
}
