#include "Burner.hlsli"

//	このシェーダが受け取る引数の内容
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

//	ピクセルシェーダへ渡す引数の内容。returnする値となる
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};


// 頂点シェーダー
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    //output.Pos = mul(input.Pos, WorldViewProj);
    output.Pos = float4(input.Pos.xyz, 1);
    output.Tex = input.Tex;
    
    return output;
}
