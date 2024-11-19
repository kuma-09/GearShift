//	C++側から設定されるデータ①
cbuffer ConstBuffer : register(b0)
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
    float4 diffuse;
};


//	このシェーダが引数として受け取るデータ
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 LPos : TEXCOORD1;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output;
    output = float4(1 - cos(input.LPos.y), 1 - input.LPos.y * 0.1f, 1, 1);
    output.a = 1 - input.LPos.y;
    output.a = output.a + cos(input.LPos.x * 10 + diffuse.x * 10);
	
    return output;

}