#include "Common.hlsli"

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texture : TEXCOORD;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
     
    output.Position = float4(input.Position, 1);
    output.Texture = input.Texture;
    
    return output;
}
