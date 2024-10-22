#include "Burner.hlsli"

//	���̃V�F�[�_���󂯎������̓��e
struct VS_INPUT
{
    float4 Pos : POSITION;
};

//	�s�N�Z���V�F�[�_�֓n�������̓��e�Breturn����l�ƂȂ�
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};


// ���_�V�F�[�_�[
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.Pos = mul(input.Pos, WorldViewProj);
    
    return output;
}
