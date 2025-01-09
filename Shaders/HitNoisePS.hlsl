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


float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float noise(float2 uv)
{
    float2 p = floor(uv);
    return random(p);
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = tex.Sample(samLinear, input.tex);
    input.tex.x += (random(floor(input.tex.y * 100) + time * 4) - 0.5f) * 0.01f;
    float4 output2 = tex.Sample(samLinear, input.tex);
    float n = noise(input.tex * float2(10, 10));
    output2.a = n - sin(time * 4);
    
    return output2;
}