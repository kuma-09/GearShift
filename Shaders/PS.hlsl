
struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//	İ’è‚³‚ê‚½‰æ‘œ‚ğ•\¦
	float4 output  =  tex.Sample(samLinear, input.Tex);
	
    
	return output;

}