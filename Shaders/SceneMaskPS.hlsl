
cbuffer ConstantBuffer : register(b0)
{
    float time;
}

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

// UVç¿ïWÇâÒì]Ç≥ÇπÇÈä÷êî
float2 rotateUV(float angle,float2 inputUV)
{
    half angleCos = cos(angle);
    half angleSin = sin(angle);
    half2x2 rotateMatrix = half2x2(angleCos, -angleSin, angleSin, angleCos);
    half2 uv = inputUV - 0.5f;
    return mul(uv, rotateMatrix) + 0.5f;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = float4(0,0,0,1);
    
    input.Tex = rotateUV(45 * 3.14f / 180, input.Tex);
    
    float horizon = floor(input.Tex.y * 5) * 0.5f;
    output.a = max(input.Tex.x - 1 + time * 5 - horizon, 0);
    
    return output;
}