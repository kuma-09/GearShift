#include "Common.hlsli"

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
    matrix lightView[4];
    matrix lightProj[4];
    int lightNum;
    float3 lightPos[64];
    float3 lightColor[64];
}

cbuffer Parameters : register(b2)
{
    float4 rimColor;
    float4 emissiveColor;
    bool isRim;
}

struct PS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color    : COLOR;
    float4 PositionWS : POSITIONT1;
};

struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float  Depth  : SV_Target2;
};

static const int pattern[4][4] =
{
    { 0, 32, 8, 40 },
    { 48, 16, 56, 24 },
    { 12, 44, 4, 36 },
    { 60, 28, 52, 20 },
};

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

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
    
    if(isRim)
    {
        // リムライト----------------------------
        float3 toEye = normalize(EyePosition - objectPos.xyz);
        half rim = 1.0 - saturate(dot(normalize(input.Normal), normalize(toEye)));
        //rim = step(0.5f, rim);
        // ---------------------------------------
        output.Albedo = DiffuseColor + rim * rimColor;
        //finalColor = lerp(finalColor, float3(1, 0, 0), rim);
        output.Albedo = float4(lerp(DiffuseColor.rgb, rimColor.rgb, rim), 1);
    }
    else
    {
        // テクスチャカラー
        output.Albedo = DiffuseColor;
    }
    

    
    // 自己発光
    output.Albedo = output.Albedo + emissiveColor;
    // ワールドNORMAL
    output.Normal = float4(input.Normal * 0.5f + 0.5f, 1.0f);
    // 深度
    float4 position = mul(float4(input.Position.xyz, 1), matView);
    position = mul(position, matProj);
    position = input.Position;
    float depth = position.z;
    output.Depth = depth;
    
    return output;
}