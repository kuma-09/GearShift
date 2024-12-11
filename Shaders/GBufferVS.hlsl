#include "Common.hlsli"

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
    matrix lightViewProj;
    float3 lightPosition;
}

struct VS_INPUT
{
    float3 Position : Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color    : COLOR;
};

struct PS_OUTPUT
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
    float4 LightDepth : Position;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output;
    
    // 頂点の位置をワールド空間へ
    output.Position = mul(float4(input.Position,1), WorldViewProj);
    // 法線ベクトルをワールド空間へ（拡大縮小の影響を打ち消すため逆転置行列をかける）
    output.Normal = mul(input.Normal, WorldInverseTranspose);
    // テクスチャのUV座標
    output.TexCoord   = input.TexCoord;
    // 色情報
    output.Color      = input.Color;
    
    output.LightDepth = mul(float4(input.Position, 1), World);
    output.LightDepth = mul(output.LightDepth, lightViewProj);

    return output;
}
