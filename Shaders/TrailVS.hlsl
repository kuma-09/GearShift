cbuffer Parmetaers : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
}

struct VS_INPUT
{
    float3 Position : Position;
    float2 Texture : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD0;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    output.Position = mul(float4(input.Position, 1), world);
    output.Position = mul(output.Position, view);
    output.Position = mul(output.Position, proj);
    output.Texture = input.Texture;
    
    return output;
}
