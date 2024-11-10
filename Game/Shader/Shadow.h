#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"


class Shadow
{
public:
	Shadow();
	~Shadow();

	void Initialize();
	void BeginDepth();
	void RenderDepth();
	void EndDepth();

	void Draw(bool texture,DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void End();

	ID3D11ShaderResourceView* GetSRV() { return m_srv; }

private:

	Graphics* m_graphics;

	// デバッグ用
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// サンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

private:

	// ライト------------------
	// ライトの位置
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ライトの回転
	DirectX::SimpleMath::Quaternion m_lightRotate;

	// ライトの範囲の角度
	float m_lightTheta;

private:
	// シェーダー--------------
	// 頂点シェーダー(深度用)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// ピクセルシェーダー(深度用)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Tex;

private:
	// 定数バッファ-------------
	// 定数バッファの構造体
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR lightPosition;	// ライトの位置
		DirectX::XMVECTOR lightDirection;	// ライトの方向
		DirectX::XMVECTOR lightAmbient;		// ライトの環境光
		DirectX::XMVECTOR color;			// モデルの色
	};

	// 定数バッファの構造体
	struct ConstantBuffer2
	{
		float fCosTheta;		// ライトのfov/2
		float pad[3];
	};

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer2;

private:

	ID3D11RenderTargetView* m_rtv;
	ID3D11ShaderResourceView* m_srv;
	ID3D11DepthStencilView* m_dsv;

	// シャドウマップテクスチャ---
	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE = 2048;

	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// シャドウマップ用（デプスステンシル）
	std::unique_ptr<DepthStencil> m_shadowMapDS;
};
