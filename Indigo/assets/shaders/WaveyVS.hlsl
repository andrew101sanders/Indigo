#include "assets/shaders/ShaderCommon.hlsli"

VSOut VSmain(VSIn i)
{
    VSOut output;

    i.positionLocal = mul(i.positionLocal, transformMatrix);
    i.positionLocal.y = (cos( (i.positionLocal.x + Time)/2 ) * .5) + (cos( (i.positionLocal.z + Time) / 2 ) * .5) -1.5;
    output.positionLocal = i.positionLocal;

    float4 transformedPosition = mul(i.positionLocal, worldMatrix);
    transformedPosition = mul(transformedPosition, viewMatrix);
    transformedPosition = mul(transformedPosition, projectionMatrix);
    output.positionClip = transformedPosition;

    output.uv = i.uv;

    output.normals = i.normals; 

    return output;
} 