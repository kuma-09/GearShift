
struct PS_INPUT
{
    float4 Position : SV_Position;
    float2 Texture : TEXCOORD0;
};


float4 main(PS_INPUT input) : SV_Target
{
    float4 output;
    //output = Texture.Sample(Sampler, input.Texture);
    output = float4(0.4f, 0.6f, 1, 1);
    output.a = 1 - input.Texture.y;
    return output;
}
