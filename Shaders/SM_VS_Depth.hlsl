#include "Common.hlsli"

// 追加する定数バッファ
cbuffer Parameters : register(b1)
{
    float4x4 LightViewProj : packoffset(c0);
    float3 LightPos : packoffset(c4);
    float4 Color : packoffset(c5);
};

struct VS_INPUT
{
    float4 Position : Position;
};

struct Output
{
    float4 Position : SV_Position;
};

Output main(VS_INPUT input)
{
    Output output;
    float4x4 perspectiveMatrix;
    // 頂点の位置を投影空間へ
    output.Position = mul(input.Position, WorldViewProj);
    
    // 頂点の位置をワールド空間へ
    //output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, LightViewProj);
    
    return output;
}
