//	C++から受け取るデータ
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 diffuse;

};

//	このシェーダが受け取る引数の内容
struct VS_INPUT
{
    float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
    float3 Normal: NORMAL;
    float3 Tangent: TANGENT;
    float3 BiNormal: BINORMAL;
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


    // 頂点座標の変換
    input.Pos += input.Normal;
    output.Pos = mul(float4(input.Pos, 1), matWorld);
    
    output.Pos = mul(output.Pos, matView);
    output.Pos = mul(output.Pos, matProj);

    


    // テクスチャ座標の渡し方は変更がないため、そのままコピー
    output.Tex = input.Tex;


    return output;
}
