#include "assets/shaders/ShaderCommon.hlsli"

float4 PSmain(VSOut i) : SV_TARGET 
{ 
    float4 textureColor = textureMap.Sample(samplerState, i.uv);
    return textureColor;
}