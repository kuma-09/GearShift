
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

// 頂点シェーダー
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;


    //// 頂点座標の変換
    output.Pos = float4(input.Pos, 1);

    // テクスチャ座標はそのまま
    output.Tex = input.Tex;


    return output;
}
