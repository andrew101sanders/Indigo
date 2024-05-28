#include "assets/shaders/ShaderCommon.hlsli"

float4 PSmain(VSOut i) : SV_TARGET 
{ 
    //return float4(1,1,1,1);
    //float hp = cos(Time)/2+.5;
    //float3 healthbarColor = lerp(float3(1,0,0), float3(0,1,0), hp);
    //float3 bgColor = float3(0,0,0);
    //float healthbarMask = hp > i.uv.x;
    ////float3 outColor = lerp(bgColor, healthbarColor, healthbarMask);
    //float3 outColor = healthbarColor * healthbarMask;
    //return float4(outColor, 0);
    //return float4(i.uv, 0, 1);
    //float2 circleCenter = float2(cos(Time), sin(Time/4));
    //float radius = 0.4; // You can adjust this value to the desired radius

    //float dist = length(i.positionLocal.xz - circleCenter);
    
    //if (dist <= radius)
    //{
    //    discard;  // Don't render the pixels in the hole in this pass
    //    return float4(0.0, 0.0, 0.0, 0.0);  // This line is redundant but satisfies the return requirement
    //}
    //else
    //{
    //    return float4(i.normals, 1);  // Render the sphere's pixels outside the hole normally
    //}

    //return float4(i.normals * (i.positionLocal.y < (cos(Time))), 1);
    i.uv.y = 1-i.uv.y;
    bool removeTopAndBottom = (abs(i.normals.y) < .99);
    //return float4(.1, .7, .1, .5); 
    float xOffset = cos( i.uv.x * TAU * 8) * 0.1; 
    float waves = cos( ( i.uv.y + xOffset + Time * .1 ) * TAU * 5) * 0.5 + 0.5; 
    //return waves; 
    float4 gradient = ColorA + i.uv.y * (ColorB - ColorA);
    return gradient * waves; 

    //return float4(i.normals, 1)*removeTopAndBottom;
    //return float4(1.0, 0.0, 0.0, 1.0);

    //float4 blendedColor = lerp(_ColorA, _ColorB, (cos(Time*4)/2+.5)); // Blend between _ColorA and _ColorB based on Time
    //return blendedColor;
}

//struct appdata
//{
//    float4 vertex : POSITION;
//    float3 normals : NORMAL;
//    float2 uv0 : TEXCOORD0;
//};

//struct v2f
//{
//    float2 uv : TEXCOORD0;
//    float3 normals : TEXCOORD1;
//    float3 pos : TEXCOORD2;
//    float4 vertex : SV_POSITION;
//};

//float4 _ColorA;
//float4 _ColorB;
//float _Rate;
//float _WaveOffset;

//float2 randomNum (in float2 uv)
//{
//    return frac(sin(dot(uv, float2(12.9898, 4.1414))) * 43758.5453);
//}