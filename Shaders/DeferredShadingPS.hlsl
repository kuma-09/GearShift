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

float readShadowMap(float3 eyeDir);

float readShadowMap(float3 eyeDir);

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
	
    
    
	// diffuse------------------------------
    float3 toLight = normalize(-LightDirection[0]);
    float intensity1 = max(dot(normal, toLight), 0.0f);
    float3 diffuse = albedo.rgb * LightDiffuseColor[0] * intensity1 + 0.65f;
	// -------------------------------------
    
	// specular-----------------------------
    float toEye = normalize(EyePosition - Position);
    float3 halfVector = normalize(toLight + toEye);
    float intensity2 = max(dot(normal, halfVector), 0.0f);
    float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
	// -------------------------------------
    
    //float3 cameraPosition = float3(0,30,0);
    //float3 eyeDir = Position.xyz - cameraPosition;
    //float3 reflectedEyeVector = normalize(reflect(eyeDir, normal));

    //float shadow = readShadowMap(eyeDir);
    //float diffuseLight = max(dot(normal, toLight), 0) * shadow;
    //float ambientLight = 0.5;

    //float3 finalColor = (diffuseLight + ambientLight) * albedo + pow(max(dot(toLight, reflectedEyeVector), 0.0), 100);
    
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

float readShadowMap(float3 eyeDir)
{
    //matrix cameraViewToWorldMatrix = inverse(mul(World, matView));
    //matrix cameraViewToProjectedLightSpace = mul(mul(mul(lightView, lightProj), mul(World, lightView)), cameraViewToWorldMatrix);
    //float4 projectedEyeDir = mul(float4(eyeDir, 1), cameraViewToProjectedLightSpace);
    //projectedEyeDir = projectedEyeDir / projectedEyeDir.w;

    //float2 textureCoordinates = projectedEyeDir.xy * float2(0.5, 0.5) + float2(0.5, 0.5);

    //const float bias = 0.0001;
    //float depthValue = ShadowMap.Sample(Sampler, textureCoordinates) - bias;
    //return projectedEyeDir.z * 0.5 + 0.5 < depthValue;
    
    // ワールド空間からライトの射影空間への行列を計算
    float4x4 worldToLightViewMatrix = mul(lightView, World);
    float4x4 lightViewToProjectionMatrix = lightProj;
    float4x4 cameraViewToWorldMatrix = inverse(matView);

    // カメラ空間からライトの射影空間への最終行列
    float4x4 cameraViewToProjectedLightSpace = mul(lightViewToProjectionMatrix, mul(worldToLightViewMatrix, cameraViewToWorldMatrix));

    // 座標を変換して正規化
    float4 projectedEyeDir = mul(cameraViewToProjectedLightSpace, float4(eyeDir, 1.0));
    projectedEyeDir /= projectedEyeDir.w;

    // テクスチャ座標を計算
    float2 textureCoordinates = projectedEyeDir.xy * float2(0.5, 0.5) + float2(0.5, 0.5);

    // シャドウマップから値を取得
    const float bias = 0.0001;
    float depthValue = ShadowMap.Sample(Sampler, textureCoordinates) - bias;

    // 深度比較
    return (projectedEyeDir.z * 0.5 + 0.5) < depthValue;
}

