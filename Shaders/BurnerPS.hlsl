#include "Burner.hlsli"

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = float4(1, 0, 0, 1);
    output.a = input.tex.x;
	
    return output;
}