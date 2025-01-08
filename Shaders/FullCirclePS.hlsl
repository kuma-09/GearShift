cbuffer ConstBuffer : register(b0)
{
    float rotate;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = tex.Sample(samLinear, input.tex);
    
    input.tex.x -= 0.5f;
    input.tex.y -= 0.5f;
    
    float angle = atan2(-input.tex.y, -input.tex.x);
    
    angle += 3.14;
    
    if(angle < rotate * 4)
    {
        discard;
    }
    if (angle >  3.14)
    {
        //discard;
    }
    float gb = rotate - 0.7f;
    
    // i‚İ‹ï‡‚ÅF•ÏX
    output = output * input.color * float4(1, 1 - gb / 1.5f, 1 - gb, 1);
    return output;
}
