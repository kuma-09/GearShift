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
    
    angle += 3.14 - 3.14 * 1 / 4;
    
    if(angle < rotate)
    {
        discard;
    }
    if (angle >  3.14 * 1 / 2)
    {
        discard;
    }
    float gb = rotate - 0.7f;
    //gb = min(gb, 1);
    output = output * float4(1, 1 - gb / 1.5f, 1 - gb, 1);
    return output;
}
