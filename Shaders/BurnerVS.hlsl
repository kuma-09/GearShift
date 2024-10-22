#include "Burner.hlsli"

//	このシェーダが受け取る引数の内容
struct VS_INPUT
{
    float4 Pos : POSITION;
};

//	ピクセルシェーダへ渡す引数の内容。returnする値となる
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};


// 頂点シェーダー
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    output.Pos = mul(input.Pos, WorldViewProj);
    
    return output;
}
