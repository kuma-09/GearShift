#include "Burner.hlsli"

//	���̃V�F�[�_���󂯎������̓��e
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

//	�s�N�Z���V�F�[�_�֓n�������̓��e�Breturn����l�ƂȂ�
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};


// ���_�V�F�[�_�[
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    //output.Pos = mul(input.Pos, WorldViewProj);
    output.Pos = float4(input.Pos.xyz, 1);
    output.Tex = input.Tex;
    
    return output;
}
