#include "Common.hlsli"

Texture2D<float4> AlbedoMap : register(t1);
Texture2D<float4> NormalMap : register(t2);
Texture2D<float4> DepthMap  : register(t3);
Texture2D<float4> ShadowMap0 : register(t4);
Texture2D<float4> ShadowMap1 : register(t5);
Texture2D<float4> ShadowMap2 : register(t6);
Texture2D<float4> ShadowMap3 : register(t7);

// �V���h�E�}�b�v�p�e�N�X�`���T���v���[
SamplerState ShadowMapSampler : register(s1);

static const float att0 = 0.0f;
static const float att1 = 0.1f;
static const float att2 = 0.0f;

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
    matrix lightView[4];
    matrix lightProj[4];
    int lightNum;
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

// �[�x�l���烏�[���h���W���v�Z
float3 ReconstructWorldPositionFromDepth(float2 uv, float depth);

// �[�x�l���J������near,far���Q�Ƃ���0~1�ɒ���
float LinearizeDepth(float depth, float near, float far);

// �V���h�E�}�b�v����e�ɂȂ��Ă��邩����
float readShadowMap(float3 worldPos);

// �o�C�A�X�l���v�Z
float CalculateShadowBias(float lightViewDepth, float slopeScale, float constantBias);

// ���ꂼ��̍s�񂩂�UV���v�Z
float3 conputeLightPosition(float4x4 viewProj, float3 worldPos);

// ���U�V���h�E�}�b�v
float VSM_Filter(float2 depth, float fragDepth);

float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`���J���[
	float3 albedo = AlbedoMap.Sample(Sampler, input.Texture).rgb;
    // �m�[�}��
    float3 normal = NormalMap.Sample(Sampler, input.Texture).rgb;
    normal = normal * 2.0f - 1.0f;
    // �[�x�l
	float depth = DepthMap.Sample(Sampler, input.Texture).r;
    // ���[���h���W
	float3 Position = ReconstructWorldPositionFromDepth(input.Texture, depth);
    // -------------------------------------
    
    //float3x3 rotation = float3x3(matView[0].xyz, matView[1].xyz, matView[2].xyz);
    //float3 translation = matView[3];
    //float3 eyePosition = mul(translation, transpose(rotation));
    //float3 eyePosition = inverse(matView)[3].xyz;
    
	// �g�U���ˌ�------------------------------
    float3 toLight = normalize(-LightDirection[0]);
    float intensity1 = max(dot(normal, toLight), 0.0f);
    float3 diffuse = albedo.rgb * toLight * intensity1 + 0.5f;
	// -------------------------------------
    
	//// ���ʔ���-----------------------------
 //   float toEye = normalize(eyePosition - Position.xyz);
 //   float3 halfVector = normalize(toLight + toEye);
 //   float intensity2 = max(dot(normal, halfVector), 0.0f);
 //   float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
	//// -------------------------------------

    //// shadow-------------------------------
    //float shadow = VSM_Filter(depth, input.Texture, depth.r);
    //shadow = readShadowMap(shadow,Position);
    float shadow = readShadowMap(Position);
    //// -------------------------------------

    //// �������C�g----------------------------
    //float3 toEye = normalize(EyePosition - Position.xyz);
    //half rim = 1.0 - saturate(dot(normalize(normal), normalize(toEye)));
    //rim = step(0.5f, rim);
    // ---------------------------------------
    float3 finalColor = albedo.rgb * diffuse * shadow;
    //finalColor += rim * float3(1, 1, 1);
    //finalColor = lerp(finalColor, float3(1, 0, 0), rim);
    
    for (int i = 0; i < lightNum; i++)
    {
        // �g�U���ˌ�
        float3 lightDirection = Position.xyz - lightPos[i].xyz;
        float3 toLight = normalize(-lightDirection);
        float lightDistance = length(lightDirection);
        float intensity1 = max(dot(normal, toLight), 0.0f);
        float3 diffuse = albedo.rgb * lightColor[i] * intensity1;
        //// ���ʔ���
        //float toEye = normalize(EyePosition - Position.xyz);
        //float3 halfVector = normalize(toLight + toEye);
        //float intensity2 = max(dot(normal, halfVector), 0.0f);
        //float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
    

        
        // ���̌���
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

float readShadowMap(float3 worldPos)
{
    float bias = 0.000005f;
    float percentLit = 1.0f;
    
    // ���C�g����̓��e��Ԃɂ���
    float4x4 lightViewProj = mul(lightView[0], lightProj[0]);
    float3 LightPosPS0 = conputeLightPosition(lightViewProj, worldPos);
    
    lightViewProj = mul(lightView[1], lightProj[1]);
    float3 LightPosPS1 = conputeLightPosition(lightViewProj, worldPos);
    
    lightViewProj = mul(lightView[2], lightProj[2]);
    float3 LightPosPS2 = conputeLightPosition(lightViewProj, worldPos);
    
    lightViewProj = mul(lightView[3], lightView[3]);
    float3 LightPosPS3 = conputeLightPosition(lightViewProj, worldPos);
    
    float2 uv0 = (LightPosPS0.xy) * float2(0.5f, -0.5f) + 0.5f;
    float2 uv1 = (LightPosPS1.xy) * float2(0.5f, -0.5f) + 0.5f;
    float2 uv2 = (LightPosPS2.xy) * float2(0.5f, -0.5f) + 0.5f;
    float2 uv3 = (LightPosPS3.xy) * float2(0.5f, -0.5f) + 0.5f;

    if (uv0.x > 0 && uv0.x < 1 && uv0.y > 0 && uv0.y < 1)
    {
        if (ShadowMap0.Sample(ShadowMapSampler, uv0).r < LightPosPS0.z - CalculateShadowBias(LightPosPS0.z, bias / 100, bias))
        {
            percentLit = 0.5f;
        }   
        //percentLit = VSM_Filter(ShadowMap0.Sample(ShadowMapSampler, uv).rg, LightPosPS.z);
        //percentLit *= 0.5f;
        //percentLit += 0.5f;
        //return percentLit;
    }
    else if (uv1.x > 0 && uv1.x < 1 && uv1.y > 0 && uv1.y < 1)
    {
        if (ShadowMap1.Sample(ShadowMapSampler, uv1).r < LightPosPS1.z - CalculateShadowBias(LightPosPS1.z, bias / 100, bias))
        {
            percentLit = 0.5f;
        }
        //percentLit = VSM_Filter(ShadowMap1.Sample(ShadowMapSampler, uv1).rg, LightPosPS1.z);
        //percentLit *= 0.5f;
        //percentLit += 0.5f;
        //return percentLit;
    }
    else if (uv2.x > 0 && uv2.x < 1 && uv2.y > 0 && uv2.y < 1)
    {
        if (ShadowMap2.Sample(ShadowMapSampler, uv2).r < LightPosPS2.z - CalculateShadowBias(LightPosPS2.z, bias / 100, bias))
        {
            percentLit = 0.5f;
        }
        //percentLit = VSM_Filter(ShadowMap2.Sample(ShadowMapSampler, uv2).rg, LightPosPS2.z);
        //percentLit *= 0.5f;
        //percentLit += 0.5f;
        //return percentLit;
    }
    else if (uv3.x > 0 && uv3.x < 1 && uv3.y > 0 && uv3.y < 1)
    {
        if (ShadowMap3.Sample(ShadowMapSampler, uv3).r < LightPosPS3.z - CalculateShadowBias(LightPosPS3.z, bias / 100, bias))
        {
            percentLit = 0.5f;
        }
        //percentLit = VSM_Filter(ShadowMap3.Sample(ShadowMapSampler, uv3).rg, LightPosPS3.z);
        //percentLit *= 0.5f;
        //percentLit += 0.5f;
        //return percentLit;
    }
    
    return max(percentLit, 0);
}

float CalculateShadowBias(float lightViewDepth, float slopeScale, float constantBias)
{
    float depthSlope = abs(ddx(lightViewDepth)) + abs(ddy(lightViewDepth));
    return slopeScale * depthSlope + constantBias;
}

float3 conputeLightPosition(float4x4 viewProj, float3 worldPos)
{
    // ���C�g����̓��e��Ԃɂ���
    float4 LightPosPS = mul(float4(worldPos, 1), viewProj);
    LightPosPS.xyz /= LightPosPS.w;
    return LightPosPS;
}

float VSM_Filter(float2 depth, float fragDepth)
{
    
    float lit = 1.0f;
    int shadowHardness = 4;
    
    //if(fragDepth > depth.r)
    {
        float depth_sq = depth.x * depth.x;
        float variance = depth.y - depth_sq;
        variance = max(depth.y - depth_sq, 0.0000005f);
        float md = fragDepth - depth.x;
        float p = variance / (variance + (md * md));
        lit = max(p, 0.0f);
        lit = pow(lit, shadowHardness);
    }
    
    return lit;
    //return saturate(max( lit, depth.r < fragDepth ));	
}
