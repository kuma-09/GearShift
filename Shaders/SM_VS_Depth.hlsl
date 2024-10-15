#include "SM_Common.hlsli"

struct VS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};

struct Output
{
    float4 Position : SV_Position;
};

Output main(VS_INPUT input)
{
    Output output;
     
    // ���_�̈ʒu�𓊉e��Ԃ�
    output.Position = mul(input.Position, WorldViewProj);
    
    // ���_�̈ʒu�����[���h��Ԃ�
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, LightViewProj);
    
    return output;
}
