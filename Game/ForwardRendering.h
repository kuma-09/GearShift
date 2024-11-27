#pragma once
#include "Framework/RenderTexture.h"

class ForwardRendering
{
public:
	static void Initialize();
	static void BeginBuffer();

	static DX::RenderTexture* GetRenderTexture() { return m_renderTexture.get(); }
private:
	// レンダーテクスチャ
	static std::unique_ptr<DX::RenderTexture> m_renderTexture;
	//// GBuffer書き込み用のシェーダー
	//static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader_tex;
	//// DeferredLighting時のシェーダー
	//static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader_light;
	//static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader_light;
	////定数バッファ
	//struct ConstBuffer
	//{
	//	DirectX::XMMATRIX matView;
	//	DirectX::XMMATRIX matProj;
	//	DirectX::XMMATRIX inverseViewProj;
	//};
	//static Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	//// インプットレイアウト
	//static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayoutGBuffer;
	//static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayoutLight;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT_G;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT_L;
	//// プリミティブバッチ
	//static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//// 頂点情報
	//static DirectX::VertexPositionTexture m_vertex[4];
};

