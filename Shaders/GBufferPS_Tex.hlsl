#include "Common.hlsli"

cbuffer Parameters : register(b1)
{
    matrix matView;
    matrix matProj;
    matrix inverseViewProj;
}

struct PS_INPUT
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Color : COLOR;
};

struct PS_OUTPUT
{
    float4 rt0 : SV_Target0;
    float4 rt1 : SV_Target1;
    float4 rt2 : SV_Target2;
};

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

// ���C���֐�
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    // �e�N�X�`���J���[
    output.rt0 = Texture.Sample(Sampler, input.TexCoord);
    // ���[���hNORMAL
    output.rt1 = normalize(float4(input.Normal, 1.0f));
    // �[�x
    float4 position = mul(float4(input.Position.xyz, 1), matView);
    position = mul(position, matProj);
    position = input.Position;
    float depth = position.z;
    output.rt2 = float4(depth, 0, 0, 0);
    
    return output;
}
