#include "Common.hlsli"

struct VS_INPUT
{
    float3 Position : Position;
    float2 Texture : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD0;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    output.Position = mul(float4(input.Position, 1), WorldViewProj);
    output.Texture = input.Texture;
    
    return output;
}
