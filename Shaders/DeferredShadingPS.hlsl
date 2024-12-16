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

// �[�x�l���烏�[���h���W���v�Z
float3 ReconstructWorldPositionFromDepth(float2 uv, float depth);

// �[�x�l���J������near,far���Q�Ƃ���0~1�ɒ���
float LinearizeDepth(float depth, float near, float far);

float readShadowMap(float3 eyeDir);

float ComputeScreenSpaceShadow(float3 worldPos, float3 lightDir);

float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`���J���[
	float3 albedo = AlbedoMap.Sample(Sampler, input.Texture).rgb;
    // �m�[�}��
    float3 normal = NormalMap.Sample(Sampler, input.Texture).rgb;
    // �[�x�l
	float depth = 1 - DepthMap.Sample(Sampler, input.Texture).r;
    // ���C�g����̐[�x�l
    float lightDepth = ShadowMap.Sample(Sampler, input.Texture).r;
    // ���[���h���W
	float3 Position = ReconstructWorldPositionFromDepth(input.Texture, depth);
	
    
    
	// diffuse------------------------------
    float3 toLight = normalize(-LightDirection[0]);
    float intensity1 = max(dot(normal, toLight), 0.0f);
    float3 diffuse = albedo.rgb * LightDiffuseColor[0] * intensity1 + 0.65f;
	// -------------------------------------
    
	// specular-----------------------------
    float toEye = normalize(EyePosition - Position.xyz);
    float3 halfVector = normalize(toLight + toEye);
    float intensity2 = max(dot(normal, halfVector), 0.0f);
    float3 specular = pow(intensity2, SpecularPower) * SpecularColor;
	// -------------------------------------
    
    matrix lightViewToWolrdMatrix = inverse(lightView);
    float3 light = lightViewToWolrdMatrix[3].xyz;
    float3 lightDir = light - Position.xyz;

    normal = normalize(normal);
    lightDir = normalize(lightDir);

    float3 eyeDir = Position.xyz - EyePosition;
    float3 reflectedEyeVector = normalize(reflect(eyeDir, normal));

    float shadow = readShadowMap(eyeDir);
    float diffuseLight = max(dot(normal, lightDir), 0) * shadow;
    float ambientLight = 0.1;
    shadow = ComputeScreenSpaceShadow(Position, toLight);
    //float3 finalColor = (diffuseLight + ambientLight) * albedo + pow(max(dot(toLight, reflectedEyeVector), 0.0), 100);
    diffuseLight = max(dot(normal, lightDir), 0) * shadow;
    ambientLight = 0.1;
    
    
    float3 finalColor = albedo.rgb * diffuse + specular;
    finalColor *= shadow;
    
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
    //matrix cameraViewToWorldMatrix = inverse(matView);
    //matrix cameraViewToProjectedLightSpace = mul(lightProj, mul(lightView, cameraViewToWorldMatrix));
    //float4 projectedEyeDir = mul(cameraViewToProjectedLightSpace, float4(eyeDir, 1));
    //projectedEyeDir = projectedEyeDir / projectedEyeDir.w;

    //float2 textureCoordinates = projectedEyeDir.xy * float2(0.5, 0.5) + float2(0.5, 0.5);

    //const float bias = 0.0001;
    //float depthValue = ShadowMap.Sample(Sampler, textureCoordinates) - bias;
    //return projectedEyeDir.z * 0.5 + 0.5 < depthValue;
    
    // ���[���h��Ԃ��烉�C�g�̎ˉe��Ԃւ̍s����v�Z
    float4x4 worldToLightViewMatrix = mul(lightView, World);
    float4x4 lightViewToProjectionMatrix = lightProj;
    float4x4 cameraViewToWorldMatrix = inverse(matView);

    // �J������Ԃ��烉�C�g�̎ˉe��Ԃւ̍ŏI�s��
    float4x4 cameraViewToProjectedLightSpace = mul(lightViewToProjectionMatrix, mul(worldToLightViewMatrix, cameraViewToWorldMatrix));

    // ���W��ϊ����Đ��K��
    float4 projectedEyeDir = mul(cameraViewToProjectedLightSpace, float4(eyeDir, 1.0));
    projectedEyeDir /= projectedEyeDir.w;

    // �e�N�X�`�����W���v�Z
    float2 textureCoordinates = projectedEyeDir.xy * float2(0.5, 0.5) + float2(0.5, 0.5);

    // �V���h�E�}�b�v����l���擾
    const float bias = 0.001;
    float depthValue = ShadowMap.Sample(Sampler, textureCoordinates) - bias;

    // �[�x��r
    return (projectedEyeDir.z * 0.5 + 0.5) < depthValue;
}

float ComputeScreenSpaceShadow(float3 worldPos, float3 lightDir) {
    float shadow = 1.0; // �f�t�H���g�ł͉e�Ȃ�
    float4 samplePos;
    
        samplePos.xyz = worldPos;
        samplePos = mul(float4(samplePos.xyz, 1), lightView);
        samplePos = mul(samplePos, lightProj);
        float2 uv = samplePos.xy / samplePos.w;
        //uv = uv * 0.5 + 0.5;
        uv = (uv.x - 0.5f) * 2, ((uv.y * -1) + 0.5f) * 2;
    
    // �T���v���ʒu�̐[�x�l���擾
        float sampleDepth = ShadowMap.Sample(Sampler, uv).r;

    // �T���v���ʒu�������̐[�x����������ΎՕ�����Ă���
        if (sampleDepth < samplePos.z - 0.0001f)
        {
            shadow = 0.5; // �e
        }

    return shadow;
}


