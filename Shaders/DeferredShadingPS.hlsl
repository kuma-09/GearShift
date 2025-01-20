#include "Common.hlsli"

Texture2D<float4> AlbedoMap : register(t1);
Texture2D<float4> NormalMap : register(t2);
Texture2D<float4> DepthMap  : register(t3);
Texture2D<float4> ShadowMap : register(t4);

// シャドウマップ用テクスチャサンプラー
SamplerState ShadowMapSampler : register(s1);

static const float att0 = 0.0f;
static const float att1 = 0.1f;
static const float att2 = 0.0f;

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
    matrix lightView;
    matrix lightProj;
    int    lightNum;
    float3 lightPos[128];
    float3 lightColor[128];
}

cbuffer Parmaeters : register(b2)
{
    float4 fog;
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

// シャドウマップから影になっているか判定
float readShadowMap(float3 worldPos,float fragDepth);

// バイアス値を計算
float CalculateShadowBias(float lightViewDepth, float slopeScale, float constantBias);

// 分散シャドウマップ
float VSM_Filter(float2 texcoord, float fragDepth);

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
    // -------------------------------------
    
	// diffuse------------------------------
    float3 toLight = normalize(-LightDirection[0]);
    float intensity1 = max(dot(normal, toLight), 0.0f);
    float3 diffuse = albedo.rgb * toLight * intensity1 + 0.5f;
	// -------------------------------------
    
	//// specular-----------------------------
    //   float toEye = normalize(EyePosition - Position.xyz);
    //   float3 halfVector = normalize(toLight + toEye);
    //   float intensity2 = max(dot(normal, halfVector), 0.0f);
    //   float3 specular = pow(intensity2, specularPower) * specularColor;
	//// -------------------------------------

    // shadow-------------------------------
    //float shadow = VSM_Filter(depth, input.Texture, depth.r);
    //shadow = readShadowMap(shadow,Position);
    
    float shadow = readShadowMap(Position,depth);
    // -------------------------------------

    float3 toEye = normalize(EyePosition - Position.xyz);
    half rim = 1.0 - saturate(dot(normalize(normal), normalize(toEye)));
    rim = step(0.5f, rim);
    float3 finalColor = albedo.rgb * diffuse * shadow;
    //finalColor += rim * float3(1, 1, 1);
    //finalColor = lerp(finalColor, float3(1, 0, 0), rim);
    
    for (int i = 0; i < lightNum; i++)
    {
        
        // 拡散反射光
        float3 lightDirection = Position.xyz - lightPos[i].xyz;
        float3 toLight = normalize(-lightDirection);
        float lightDistance = length(lightDirection);
        float intensity1 = max(dot(normal, toLight), 0.0f);
        float3 diffuse = albedo.rgb * lightColor[i] * intensity1;
        //// 鏡面反射
        //float toEye = normalize(EyePosition - Position.xyz);
        //float3 halfVector = normalize(toLight + toEye);
        //float intensity2 = max(dot(normal, halfVector), 0.0f);
        //float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
    
        // 光の減衰
        float attenuation = 1.0f / (att0 + att1 * lightDistance + att2 * lightDistance * lightDistance);
        diffuse *= attenuation;
        //specular *= attenuation;
    
        finalColor += albedo.rgb * diffuse;
    }

    finalColor = lerp(float3(0, 0, 0), finalColor, 1 - LinearizeDepth(depth, 0.1, 200));
    
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

float readShadowMap(float3 worldPos,float fragDepth)
{
    // ライトからの投影空間にする
    float4 LightPosPS = mul(float4(worldPos, 1), lightView);
    LightPosPS = mul(LightPosPS, lightProj);
    
    LightPosPS.xyz /= LightPosPS.w;

    // 参照するシャドウマップのUV値を求める
    float2 uv = (LightPosPS.xy) * float2(0.5f, -0.5f) + 0.5f;
    
    // UV座標が有効範囲外の場合の処理
    if (uv.x > 1.0f || uv.x < 0.0f || uv.y > 1.0f || uv.y < 0.0f)
    {
        return 1.0f;
    }
    
    float bias = 0.0000005f;
    float percentLit = 1.0f;
    // シャドウマップの深度値とライト空間のピクセルのZ値を比較して影になるか調べる
    //float percentLit = VSM_Filter(uv, LightPosPS.z);
    if (ShadowMap.Sample(ShadowMapSampler, uv).r < LightPosPS.z - CalculateShadowBias(LightPosPS.z, bias / 100, bias))
    {
        percentLit = 0.5f;
    }
    
    return max(percentLit, 0);
}

float CalculateShadowBias(float lightViewDepth, float slopeScale, float constantBias)
{
    float depthSlope = abs(ddx(lightViewDepth)) + abs(ddy(lightViewDepth));
    return slopeScale * depthSlope + constantBias;
}

float VSM_Filter(float2 texcoord, float fragDepth)
{
    
    float2 depth = ShadowMap.Sample(ShadowMapSampler, texcoord);
    float lit = 1.0f;
    
    if(fragDepth > depth.r)
    {
        float depth_sq = depth.x * depth.x;
        float variance = depth.y - depth_sq;
        float md = fragDepth - depth.x;
        float p = variance / (variance + (md * md));
        lit = max(p, 0.0f);
        lit = pow(lit, 4);
    }
    
    return lit;

    //return saturate(max( p, depth.r <= fragDepth ));	
}
