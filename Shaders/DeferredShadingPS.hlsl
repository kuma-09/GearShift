#include "Common.hlsli"

Texture2D<float4> AlbedoMap : register(t1);
Texture2D<float4> NormalMap : register(t2);
Texture2D<float4> DepthMap  : register(t3);
Texture2D<float4> ShadowMap : register(t4);

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
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

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャカラー
	float3 albedo = AlbedoMap.Sample(Sampler, input.Texture).rgb;
    // ノーマル
    float3 normal = NormalMap.Sample(Sampler, input.Texture).rgb;
    // 深度値
	float depth = 1 - DepthMap.Sample(Sampler, input.Texture).r;
    // ライトからの深度値
    float lightDepth = ShadowMap.Sample(Sampler, input.Texture).r;
    // ワールド座標
	float3 Position = ReconstructWorldPositionFromDepth(input.Texture, depth);
	
    //depth = LinearizeDepth(depth, 0.1f, 100.0f);
	
	// diffuse------------------------------
    float3 toLight = normalize(-LightDirection[0]);
    float intensity1 = max(dot(normal, toLight), 0.0f);
    float3 diffuse = albedo.rgb * LightDiffuseColor[0] * intensity1 + 0.5f;
	
	// specular-----------------------------
    float toEye = normalize(EyePosition - Position);
    float3 halfVector = normalize(toLight + toEye);
    float intensity2 = max(dot(normal, halfVector), 0.0f);
    float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
	
    float3 finalColor = albedo.rgb * diffuse;
	
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