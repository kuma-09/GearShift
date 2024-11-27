#include "Common.hlsli"

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
}

struct PS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color    : COLOR;
};

struct PS_OUTPUT
{
    float4 rt0 : SV_Target0;
    float4 rt1 : SV_Target1;
    float4 rt2 : SV_Target2;
};

// メイン関数
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    // テクスチャカラー
    output.rt0 = DiffuseColor;
    // ワールドNORMAL
    output.rt1 = float4(input.Normal, 1.0f);
    // 深度
    float4 position = mul(float4(input.Position.xyz, 1), matView);
    position = mul(position, matProj);
    position = input.Position;
    float depth = position.z / position.w;
    output.rt2 = float4(depth, 0, 0, 0);
    
    return output;
}
