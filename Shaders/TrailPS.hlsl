#include "Common.hlsli"

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD0;
};


float4 main(PS_INPUT input) : SV_Target
{
    float4 output;
    output = Texture.Sample(Sampler, input.Texture);
    //output = float4(1, 1, 1, 1);
    output.a *= input.Texture.x;
    return output;
}
