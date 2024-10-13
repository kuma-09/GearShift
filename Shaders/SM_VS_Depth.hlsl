#include "SM_Common.hlsli"

struct VS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};

struct Output
{
    float4 Position : SV_Position;
};

Output main(VS_INPUT input)
{
    Output output;
     
    // ’¸“_‚ÌˆÊ’u‚ð“Š‰e‹óŠÔ‚Ö
    output.Position = mul(input.Position, WorldViewProj);
    
    return output;
}
