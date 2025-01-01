cbuffer ConstBuffer : register(b1)
{
    float time;
}

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

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    input.tex.y += floor(input.tex.y * 10) * time;
    
    float4 output = tex.Sample(samLinear, input.tex);
    
    return output;
}