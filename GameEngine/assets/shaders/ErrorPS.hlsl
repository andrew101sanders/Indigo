#include "ShaderCommon.hlsli"

float4 PSmain(VSOut i) : SV_TARGET 
{ 
	return float4(0.7,0.4,0.7,0.1);
}