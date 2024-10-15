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
     
    // 頂点の位置を投影空間へ
    output.Position = mul(input.Position, WorldViewProj);
    
    // 頂点の位置をワールド空間へ
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, LightViewProj);
    
    return output;
}
