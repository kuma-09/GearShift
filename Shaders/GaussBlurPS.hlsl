struct VS_INPUT
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float gauss(float x, float y, float sigma)
{
    return 1.0f / (2.0f * 3.14f * sigma * sigma) * exp(-(x * x + y * y) / (2.0f * sigma * sigma));
}

#define KERNEL_SIZE 35

float4 main(PS_INPUT input) : SV_Target
{
    float4 o = 0;
    float sum = 0;
    float2 uvOffset;
    float weight;
    for (int x = -KERNEL_SIZE / 2; x <= KERNEL_SIZE / 2; ++x)
        for (int y = -KERNEL_SIZE / 2; y <= KERNEL_SIZE / 2; ++y)
        {
            //オフセットを計算する
            uvOffset = input.tex;
            uvOffset.x += x * 1280.f;
            uvOffset.y += y * 720.f;
            //重みを確認する
            weight = gauss(x, y, _Sigma);
            o += tex2D(_MainTex, uvOffset) * weight;
            sum += weight;
        }
    o *= (1.0f / sum);
    return o;
}
