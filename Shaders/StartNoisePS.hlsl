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
    
    input.tex.x += sin(time) / 100 * sin(input.pos.y);
    
    
    float4 output = tex.Sample(samLinear, input.tex);
    
    float rg = 1 - time ;
    rg = min(rg, 1);
    float4 color = float4(rg,rg , 1, 1);
    color = float4(rg, 1, 1, 1);
    
    return output * color;
}