#pragma once
#include "Framework/Graphics.h"
#include "Framework/RenderTexture.h"

class OutLine
{
public:
	OutLine();
	~OutLine();

	void Initialize();
	void Update(float elapsedTime);
	void Render(ID3D11ShaderResourceView* depthTexture,ID3D11ShaderResourceView* normalTexture);

	void LoadShader(ID3D11Device* device);

private:

	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 頂点情報
	DirectX::VertexPositionTexture m_vertex[4];

	// レンダーテクスチャ
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	//// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

};
