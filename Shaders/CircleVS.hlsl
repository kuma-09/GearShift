cbuffer ConstBuffer : register(b0)
{
    float rotate;    
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    output.pos = float4(input.pos, 1);
    output.color = input.color;
    output.tex = input.tex;
    return output;
}