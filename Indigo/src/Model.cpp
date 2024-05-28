#include "pch.h"
#include "Model.h"
#include "Mesh.h"

void Model::AddMeshEntry(Mesh* mesh, Material* material)
{
    MeshEntry entry;
    mesh->SetParentModel( this );
    entry.mesh = mesh;
    entry.material = material;
    meshEntries.push_back(entry);
}

void Model::SetPosition(const DirectX::XMFLOAT3& pos)
{
    position = pos;
    UpdateTransformationMatrix();
}

void Model::SetRotation(const DirectX::XMFLOAT3& rot)
{
    rotation = rot;
    UpdateTransformationMatrix();
}

void Model::SetScale(const DirectX::XMFLOAT3& scl)
{
    scale = scl;
    UpdateTransformationMatrix();
}

void Model::UpdateTransformationMatrix()
{
    // Create individual matrices
    DirectX::XMMATRIX matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX matRotate = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    DirectX::XMMATRIX matTranslate = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    // Combine them to get the final transformation matrix
    transformationMatrix = matScale * matRotate * matTranslate;
}

XMMATRIX Model::GetTransformationMatrix()
{
    return transformationMatrix;
}

