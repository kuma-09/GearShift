cbuffer ConstBuffer : register(b0)
{
    float rotate;
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
    
    input.tex.x -= 0.5f;
    input.tex.y = (input.tex.y - 0.5f) * -1.0f;
    
    float angle = atan2(-input.tex.y, -input.tex.x);
    
    angle += 3.14f;
    
    if(angle < rotate)
    {
        discard;
    }
    return output;
}