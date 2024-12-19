#include "Common.hlsli"

Texture2D<float4> AlbedoMap : register(t1);
Texture2D<float4> NormalMap : register(t2);
Texture2D<float4> DepthMap  : register(t3);
Texture2D<float4> ShadowMap : register(t4);

// シャドウマップ用テクスチャサンプラー
SamplerComparisonState ShadowMapSampler : register(s1);

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
    matrix lightView;
    matrix lightProj;
}

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 Texture  : TEXCOORD;
};

// 深度値からワールド座標を計算
float3 ReconstructWorldPositionFromDepth(float2 uv, float depth);

// 深度値をカメラのnear,farを参照して0~1に調整
float LinearizeDepth(float depth, float near, float far);

//float readShadowMap(float3 eyeDir);
float readShadowMap(float3 worldPos);

float ComputeScreenSpaceShadow(float3 worldPos, float3 lightDir);

float CalculateShadowBias(float lightViewDepth, float slopeScale, float constantBias)
{
    float depthSlope = abs(ddx(lightViewDepth)) + abs(ddy(lightViewDepth));
    return slopeScale * depthSlope + constantBias;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャカラー
	float3 albedo = AlbedoMap.Sample(Sampler, input.Texture).rgb;
    // ノーマル
    float3 normal = NormalMap.Sample(Sampler, input.Texture).rgb;
    // 深度値
	float depth = DepthMap.Sample(Sampler, input.Texture).r;
    // ワールド座標
	float3 Position = ReconstructWorldPositionFromDepth(input.Texture, depth);
    
	// diffuse------------------------------
    float3 toLight = normalize(Position.xyz - float3(0, 50, -50));
    float intensity1 = max(dot(normal, toLight), 0.0f);
    float3 diffuse = albedo.rgb * (toLight) * intensity1 + 0.5f;
	// -------------------------------------
    
	// specular-----------------------------
    float toEye = normalize(EyePosition - Position.xyz);
    float3 halfVector = normalize(toLight + toEye);
    float intensity2 = max(dot(normal, halfVector), 0.0f);
    float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
	// -------------------------------------
    
    // 法線を正規化
    float3 worldNormal = normalize(normal);
    // 光の強さを内積から算出する
    float3 dotL = saturate(dot(toLight, worldNormal));

    float shadow = readShadowMap(Position);
    
    float3 lightColor = { 0.5f, 0.5f, 0.5f };
    
    // ライトによる明るさを求める
    //float3 lightAmount = dotL * shadow * (1.0f - lightColor) + lightColor;
    
    
    
    //float diffuseLight = max(dot(normal, toLight), 0) * shadow;
    //float ambientLight = 0.1;
    //shadow = ComputeScreenSpaceShadow(Position, toLight);
    //float3 finalColor = (diffuseLight + ambientLight) * albedo + pow(max(dot(toLight, reflectedEyeVector), 0.0), 100);
    //diffuseLight = max(dot(normal, lightDir), 0) * shadow;
    
    //diffuse = albedo.rgb * (toLight) * intensity1 + shadow;

    float3 finalColor = albedo.rgb * (diffuse * shadow) + specular;

    return float4(finalColor, 1);
}

float3 ReconstructWorldPositionFromDepth(float2 uv, float depth)
{
    float4 worldPos = float4((uv.x - 0.5f) * 2, ((uv.y * -1) + 0.5f) * 2, depth, 1.0);

    worldPos = mul(inverseViewProj, worldPos);
    return worldPos.xyz / worldPos.w;
}

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

float readShadowMap(float3 worldPos)
{
    // ライトからの投影空間にする
    float4 LightPosPS = mul(float4(worldPos, 1), lightView);
    LightPosPS = mul(LightPosPS, lightProj);
    
    LightPosPS.xyz /= LightPosPS.w;

    // 参照するシャドウマップのUV値を求める
    float2 uv = uv = (LightPosPS.xy) * float2(0.5f, -0.5f) + 0.5f;
    // UV座標が有効範囲外の場合の処理
    
    if(uv.x > 1.0f || uv.x < 0.0f || uv.y > 1.0f || uv.y < 0.0f)
    {
        return 0.5f;
    }
    
    float bias = 0.000005f;
    // シャドウマップの深度値とライト空間のピクセルのZ値を比較して影になるか調べる
    float percentLit = 1.0f;
    if (ShadowMap.Sample(Sampler, uv).r < LightPosPS.z - CalculateShadowBias(LightPosPS.z,bias,bias))
    {
        percentLit = 0.5f;
    }
    
    return percentLit;
}

float ComputeScreenSpaceShadow(float3 worldPos, float3 lightDir) 
{
    float shadow = 1.0; // デフォルトでは影なし
    float4 samplePos;
    
    samplePos = mul(float4(worldPos, 1), lightView);
    samplePos = mul(samplePos, lightProj);
    float2 uv = samplePos.xy / samplePos.w;
    uv = (uv.x - 0.5f) * 2, ((uv.y * -1) + 0.5f) * 2;
    
    // サンプル位置の深度値を取得
    float sampleDepth = ShadowMap.Sample(Sampler, uv).r;

    // サンプル位置が既存の深度よりも遠ければ遮蔽されている
    if (sampleDepth < samplePos.z - 0.0001f)
    {
        shadow = 1.0f; // 影
    }

    return shadow;
}