#include "SM_Common.hlsli"

struct VS_INPUT
{
    float4 Position     : SV_Position;
    float3 Normal       : NORMAL;
    float2 TexCoord     : TEXCOORD0;
    float4 Color        : COLOR;
};

struct PS_OUTPUT
{
    float2 TexCoord     : TEXCOORD0;
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float4 LightPosPS   : TEXCOORD3;
    float4 PositionPS   : SV_Position;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output;

    // ���_�̈ʒu�𓊉e��Ԃ�
    output.PositionPS = mul(input.Position, WorldViewProj);
    // ���_�̈ʒu�����[���h��Ԃ�
    output.PositionWS = mul(input.Position, World);
    // �@���x�N�g�������[���h��Ԃցi�g��k���̉e����ł��������ߋt�]�u�s���������j
    output.NormalWS = mul(input.Normal, WorldInverseTranspose);
    // �e�N�X�`����UV���W
    output.TexCoord = input.TexCoord;
    // ���_�̈ʒu�����C�g�̓��e��Ԃ�
    output.LightPosPS = mul(output.PositionWS, LightViewProj);
 
    return output;
}
