#include "Common.hlsli"

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
}

struct VS_INPUT
{
    float3 Position : Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color    : COLOR;
};

struct PS_OUTPUT
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
    float4 PositionWS : POSITIONT1;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output;
    
    // ���_�̈ʒu�����[���h��Ԃ�
    output.Position = mul(float4(input.Position,1), World);
    output.Position = mul(output.Position, matView);
    output.Position = mul(output.Position, matProj);
    // �@���x�N�g�������[���h��Ԃցi�g��k���̉e����ł��������ߋt�]�u�s���������j
    output.Normal = mul(input.Normal, WorldInverseTranspose);
    // �e�N�X�`����UV���W
    output.TexCoord   = input.TexCoord;
    // �F���
    output.Color      = input.Color;
    // ���[���h���
    output.PositionWS = mul(float4(input.Position, 1), World);

    return output;
}
