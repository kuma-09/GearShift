cbuffer ConstBuffer : register(b1)
{
    float value;
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
    float4 output = tex.Sample(samLinear, input.tex);
    
    if (input.tex.x > value)
    {
        discard;
    }
    return output;
}