//	C++側から設定されるデータ①
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 diffuse;
};

//	C++側から設定されるデータ②
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

//	このシェーダが引数として受け取るデータ
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	//	設定された画像を表示
	float4 output  =  tex.Sample(samLinear, input.Tex);
	float4 output2 = tex2.Sample(samLinear, input.Tex);

	//	真っ白な板ポリゴン
	float4 outputw = float4(1, 0, 0, 1);
	
	return output2 * outputw;

}