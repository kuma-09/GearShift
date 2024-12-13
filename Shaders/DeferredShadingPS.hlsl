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

float readShadowMap(float3 eyeDir);

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
    const float bias = 0.0001;
    float depthValue = ShadowMap.Sample(Sampler, textureCoordinates) - bias;

    // �[�x��r
    return (projectedEyeDir.z * 0.5 + 0.5) < depthValue;
}

