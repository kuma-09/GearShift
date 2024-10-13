#include "SM_Common.hlsli"

struct VS_INPUT
{
    float4 Position     : SV_Position;
    float3 Normal       : NORMAL;
    float2 TexCoord     : TEXCOORD0;
    float4 Color        : COLOR;
};

struct PS_OUTPUT
{
    float2 TexCoord     : TEXCOORD0;
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float4 LightPosPS   : TEXCOORD3;
    float4 PositionPS   : SV_Position;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output;

    // 頂点の位置を投影空間へ
    output.PositionPS = mul(input.Position, WorldViewProj);
    // 頂点の位置をワールド空間へ
    output.PositionWS = mul(input.Position, World);
    // 法線ベクトルをワールド空間へ（拡大縮小の影響を打ち消すため逆転置行列をかける）
    output.NormalWS = mul(input.Normal, WorldInverseTranspose);
    // テクスチャのUV座標
    output.TexCoord = input.TexCoord;
    // 頂点の位置をライトの投影空間へ
    output.LightPosPS = mul(output.PositionWS, LightViewProj);
 
    return output;
}
