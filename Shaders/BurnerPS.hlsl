#include "Burner.hlsli"

struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = float4(1, 1, 1, 1);
	
    return output;
}