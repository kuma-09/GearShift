//	C++������ݒ肳���f�[�^�@
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 diffuse;
};

//	C++������ݒ肳���f�[�^�A
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

//	���̃V�F�[�_�������Ƃ��Ď󂯎��f�[�^
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	//	�ݒ肳�ꂽ�摜��\��
	float4 output  =  tex.Sample(samLinear, input.Tex);
	float4 output2 = tex2.Sample(samLinear, input.Tex);

	//	�^�����Ȕ|���S��
	float4 outputw = float4(1, 0, 0, 1);
	
	return output2 * outputw;

}