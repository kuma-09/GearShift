//	C++����󂯎��f�[�^
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
    float4 diffuse;

};

//	���̃V�F�[�_���󂯎������̓��e
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

//	�s�N�Z���V�F�[�_�֓n�������̓��e�Breturn����l�ƂȂ�
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 LPos : TEXCOORD1;
};

// ���_�V�F�[�_�[
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    output.LPos = input.Pos;
    
    input.Pos.x = input.Pos.x * ((2 - sin(input.Pos.y)) / 5);
    input.Pos.z = input.Pos.z * ((2 - sin(input.Pos.y)) / 5);
    input.Pos.y *= 1.5f;
   
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    output.Pos = mul(output.Pos, matView);
    output.Pos = mul(output.Pos, matProj);

    // �e�N�X�`�����W���o��
    output.Tex = input.Tex;

    return output;
}
