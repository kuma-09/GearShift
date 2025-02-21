#include "Common.hlsli"

Texture2D<float4> DepthMap : register(t1);
Texture2D<float4> NormalMap : register(t2);

static const float DepthThreshold = 0.1f;
static const float NormalThreshold = 0.5f;


struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD;
};

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

float4 main(PS_INPUT input) : SV_Target
{
    
    // 深度バッファからエッジを抽出
    float depthLB = DepthMap.Sample(Sampler, input.Texture + float2(-0.001f, 0.001f)).r;
    float depthRT = DepthMap.Sample(Sampler, input.Texture + float2( 0.001f,-0.001f)).r;
    float depthRB = DepthMap.Sample(Sampler, input.Texture + float2( 0.001f, 0.001f)).r;
    float depthLT = DepthMap.Sample(Sampler, input.Texture + float2(-0.001f,-0.001f)).r;
    
    // 法線バッファからエッジを抽出
    float3 normalLB = NormalMap.Sample(Sampler, input.Texture + float2(-0.001f, 0.001f)).rgb;
    float3 normalRT = NormalMap.Sample(Sampler, input.Texture + float2( 0.001f,-0.001f)).rgb;
    float3 normalRB = NormalMap.Sample(Sampler, input.Texture + float2( 0.001f, 0.001f)).rgb;
    float3 normalLT = NormalMap.Sample(Sampler, input.Texture + float2(-0.001f,-0.001f)).rgb;
    
    depthLB = LinearizeDepth(depthLB, 0.1f, 100.0f);
    depthRT = LinearizeDepth(depthRT, 0.1f, 100.0f);
    depthRB = LinearizeDepth(depthRB, 0.1f, 100.0f);
    depthLT = LinearizeDepth(depthLT, 0.1f, 100.0f);
    
    normalLB = normalLB * 2.0f + 1.0f;
    normalRT = normalRT * 2.0f + 1.0f;
    normalRB = normalRB * 2.0f + 1.0f;
    normalLT = normalLT * 2.0f + 1.0f;
    
    float depthFiniteDifference0 = depthLT - depthRB;
    float depthFiniteDifference1 = depthRT - depthLB;
    float edgeDepth = sqrt(pow(depthFiniteDifference0, 2) + pow(depthFiniteDifference1, 2));
    edgeDepth = 1 - step(edgeDepth, DepthThreshold);
    
    float3 normalFiniteDifference0 = normalLT - normalRB;
    float3 normalFiniteDifference1 = normalRT - normalLB;
    float edgeNormal = sqrt(dot(normalFiniteDifference0, normalFiniteDifference0) + dot(normalFiniteDifference1, normalFiniteDifference1));
    edgeNormal = 1 - step(edgeNormal, NormalThreshold);
    
    
    //clip(edgeDepth - DepthThreshold);
    clip(edgeNormal - NormalThreshold);
    
    //return float4(edgeDepth,edgeDepth,edgeDepth, 1);
    return float4(edgeNormal, edgeNormal, edgeNormal, 1);

}