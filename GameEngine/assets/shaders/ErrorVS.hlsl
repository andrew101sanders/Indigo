#include "ShaderCommon.hlsli"

VSOut VSmain(VSIn i)
{
    VSOut output;

    i.positionLocal = mul(i.positionLocal, transformMatrix);
    output.positionLocal = i.positionLocal;

    float4 transformedPosition = mul(i.positionLocal, worldMatrix);
    transformedPosition = mul(transformedPosition, viewMatrix);
    transformedPosition = mul(transformedPosition, projectionMatrix);
    output.positionClip = transformedPosition;

    output.uv = i.uv * 2 - 1;

    output.normals = i.normals; 

    return output;
} 