
struct VS_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D tex1 : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 texture1 = tex1.Sample(samLinear, input.tex);
    float4 texture2 = tex2.Sample(samLinear, input.tex);
    
    float4 output = texture1 + texture2;
    
    return output;
}