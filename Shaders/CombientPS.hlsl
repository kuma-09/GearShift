
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

Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 texture1 = tex1.Sample(samLinear, input.tex);
    float4 texture2 = tex2.Sample(samLinear, input.tex);
    
    float3 output = texture1.rgb + texture2.rgb;
    
    return float4(output, 1);
}