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
#define SHADOW_EPSILON 0.000005f

float CalculateShadowBias(float lightViewDepth, float slopeScale, float constantBias)
{
    float depthSlope = abs(ddx(lightViewDepth)) + abs(ddy(lightViewDepth));
    return slopeScale * depthSlope + constantBias;
}

float4 main(Input pin) : SV_TARGET0
{
    float4 diffuse;
    float3 specular = 0;

    // ���C�g�̌��������߂�
    float3 lightDir = normalize(pin.PositionWS.xyz - LightPos.xyz);


    // ���K���f�o�C�X���W�ɂ���
    pin.LightPosPS.xyz /= pin.LightPosPS.w;

    // �Q�Ƃ���V���h�E�}�b�v��UV�l�����߂�
    float2 uv = pin.LightPosPS.xy * float2(0.5f, -0.5f) + 0.5f;
    float bias = CalculateShadowBias(pin.LightPosPS.z, 0.00005, 0.00005);
    // �V���h�E�}�b�v�̐[�x�l�ƃ��C�g��Ԃ̃s�N�Z����Z�l���r���ĉe�ɂȂ邩���ׂ�
    float percentLit = ShadowMapTexture.SampleCmpLevelZero(ShadowMapSampler, uv, pin.LightPosPS.z - bias).x;
 
    // ------------------------------------------------------------------------------- //
    // �f�B�t���[�Y
    // ------------------------------------------------------------------------------- //
 
    // �@���𐳋K��
    float3 worldNormal = normalize(pin.NormalWS);

    // ���̋�������ς���Z�o����
    float3 dotL = saturate(dot(-lightDir, worldNormal));

    // ���C�g�ɂ�閾�邳�����߂�
    float3 lightAmount = dotL * percentLit * (1.0f - LightAmbient) + LightAmbient;

    // �f�B�t���[�Y�F�����߂� 
    diffuse = float4(DiffuseColor.rgb * lightAmount, DiffuseColor.a);

    // ------------------------------------------------------------------------------- //
    // �X�y�L����
    // ------------------------------------------------------------------------------- //
    
    // �����x�N�g��
    float3 eyeVector = normalize(EyePosition - pin.PositionWS.xyz);

    // �n�[�t�x�N�g��
    float3 halfVector = normalize(eyeVector - lightDir);

    // �X�y�L�����̉e����������ς��g�����߂�
    float dotH = saturate(dot(halfVector, worldNormal));

    // �X�y�L�����p���[���w���Ƃ��Ďg���n�C���C�g�̂������𒲐�
    specular = pow(dotH, SpecularPower) * dotL * SpecularColor * percentLit;

  
    // �e�N�X�`���F�ƃf�B�t���[�Y�F���|���� 
    float4 color = Color * diffuse;

    // �X�y�L������������
    color.rgb += specular;

    return color;
}

