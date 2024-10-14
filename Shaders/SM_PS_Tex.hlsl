#include "SM_Common.hlsli"

// �V���h�E�}�b�v�e�N�X�`��
Texture2D ShadowMapTexture : register(t1);

// �V���h�E�}�b�v�p�e�N�X�`���T���v���[
SamplerComparisonState ShadowMapSampler : register(s1);

struct Input
{
    float2 TexCoord : TEXCOORD0;
    float4 PositionWS : TEXCOORD1;
    float3 NormalWS : TEXCOORD2;
    float4 LightPosPS : TEXCOORD3;
};

// �}�b�n�o���h�΍�
#define SHADOW_EPSILON 0.0005f

float4 main(Input pin) : SV_TARGET0
{
    float4 diffuse;
    float3 specular = 0;

    // ���C�g�̌��������߂�
    float3 lightDir = normalize(pin.PositionWS.xyz - LightPos.xyz);

    // �X�|�b�g���C�g�͈͓̔����H
    if (dot(lightDir, LightDir) > fCosTheta)
    {
        // ���K���f�o�C�X���W�ɂ���
        pin.LightPosPS.xyz /= pin.LightPosPS.w;

        // �Q�Ƃ���V���h�E�}�b�v��UV�l�����߂�
        float2 uv = pin.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;

        // �V���h�E�}�b�v�̐[�x�l�ƃ��C�g��Ԃ̃s�N�Z����Z�l���r���ĉe�ɂȂ邩���ׂ�
        float percentLit = ShadowMapTexture.SampleCmpLevelZero(ShadowMapSampler, uv, pin.LightPosPS.z - SHADOW_EPSILON).x;
 
        // ------------------------------------------------------------------------------- //
        // �f�B�t���[�Y
        // ------------------------------------------------------------------------------- //
 
        // �@���𐳋K��
        float3 worldNormal = normalize(pin.NormalWS);

        // ���̋�������ς���Z�o����
        float3 dotL = saturate(dot(-lightDir, worldNormal));

        // ���C�g�ɂ�閾�邳�����߂�
        float3 lightAmount = percentLit * (1.0f - LightAmbient) + LightAmbient;

        // �f�B�t���[�Y�F�����߂� 
        diffuse = float4(lightAmount, 1.0f) * DiffuseColor;
    }
    else
    {
        // �X�|�b�g���C�g�̊O
        diffuse = float4(LightAmbient, 1.0f) * DiffuseColor;
    }
  
    // �e�N�X�`���F�ƃf�B�t���[�Y�F���|���� 
    float4 color = Texture.Sample(Sampler, pin.TexCoord) * diffuse;


    return color;
}
