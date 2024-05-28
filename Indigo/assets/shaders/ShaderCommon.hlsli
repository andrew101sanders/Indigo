#define TAU 6.283185307

Texture2D textureMap : register( t0 );
SamplerState samplerState : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
    float4x4 transformMatrix;
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 ColorA;
    float4 ColorB;
    double Time;
    float padding[ 2 ];

};

struct VSIn
{
    float4 positionLocal : POSITION;
    float2 uv : TEXCOORD0;
    float3 normals : NORMAL;
};

struct VSOut
{
    float4 positionClip : SV_POSITION;
    float4 positionLocal : TEXCOORD2;
    float2 uv : TEXCOORD0;
    float3 normals : TEXCOORD1;
};