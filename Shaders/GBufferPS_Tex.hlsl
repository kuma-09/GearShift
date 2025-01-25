#include "Common.hlsli"

Texture2D<float4> ShadowMap : register(t1);

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
    matrix lightView;
    matrix lightProj;
    int lightNum;
    float3 lightPos[128];
    float3 lightColor[128];
}

struct PS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
    float4 PositionWS : POSITIONT1;
};

struct PS_OUTPUT
{
    float4 rt0 : SV_Target0;
    float4 rt1 : SV_Target1;
    float4 rt2 : SV_Target2;
};

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

static const int pattern[4][4] =
{
    { 0, 32, 8, 40 },
    { 48, 16, 56, 24 },
    { 12, 44, 4, 36 },
    { 60, 28, 52, 20 },
};

// メイン関数
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    
    float4 objectPos = input.PositionWS;
    float4 objectPosInCamera = mul(objectPos, matView);
    
    float distToEye = length(objectPosInCamera);
    
    int x = (int) fmod(input.Position.x, 4.0f);
    int y = (int) fmod(input.Position.y, 4.0f);
 
    int dither = pattern[y][x];
    
    float clipRange = 2.0f;
    
    float eyeToClipRange = max(0.0f, distToEye - clipRange);
    float clipRate = 1.0f - min(1.0f, eyeToClipRange / 5.0f);
    
    clip(dither - 64 * clipRate);
    
    // テクスチャカラー
    output.rt0 = Texture.Sample(Sampler, input.TexCoord);
    // ワールドNORMAL
    output.rt1 = float4(input.Normal, 1.0f);
    // 深度
    float4 position = mul(float4(input.Position.xyz, 1), matView);
    position = mul(position, matProj);
    position = input.Position;
    float depth = position.z;
    output.rt2 = float4(depth, 0, 0, 0);
    
    return output;
}
