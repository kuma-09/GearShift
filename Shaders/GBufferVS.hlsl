#include "Common.hlsli"

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
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
    float4 PositionWS : POSITIONT1;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output;
    
    // 頂点の位置をワールド空間へ
    output.Position = mul(float4(input.Position,1), World);
    output.Position = mul(output.Position, matView);
    output.Position = mul(output.Position, matProj);
    // 法線ベクトルをワールド空間へ（拡大縮小の影響を打ち消すため逆転置行列をかける）
    output.Normal = mul(input.Normal, WorldInverseTranspose);
    // テクスチャのUV座標
    output.TexCoord   = input.TexCoord;
    // 色情報
    output.Color      = input.Color;
    // ワールド空間
    output.PositionWS = mul(float4(input.Position, 1), World);

    return output;
}
