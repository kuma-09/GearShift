#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"


class ShadowMap
{
public:
	static void Initialize();
	static void BeginDepth(int num);
	static void RenderDepth();
	static void EndDepth();

	static void SetLightPosition(DirectX::SimpleMath::Vector3 targetPos);

	static DirectX::SimpleMath::Matrix GetLightView(int num);
	static DirectX::SimpleMath::Matrix GetLightProj(int num);

	static DX::RenderTexture* GetShadowRenderTexture(int num);
	static void ShadowMapShow();

private:
	// デバイスリソース
	static Graphics* m_graphics;

	static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;

	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// サンプラー
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// ライト------------------
	// ライトの位置
	static DirectX::SimpleMath::Vector3 m_lightPosition[4];

	// ライトの注視点
	static DirectX::SimpleMath::Vector3 m_targetPosition[4];

	// ライトの範囲の角度
	static float m_lightTheta[4];

	// シェーダー--------------
	// 頂点シェーダー(深度用)
	static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;
	// ピクセルシェーダー(深度用)
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// 定数バッファ-------------
	// 定数バッファの構造体
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR lightPosition;	// ライトの位置
		DirectX::XMVECTOR color;			// モデルの色
	};

	// 定数バッファへのポインタ
	static Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	// シャドウマップテクスチャ---
	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE_X = 1024;
	static const int SHADOWMAP_SIZE_Y = 1024;

	// シャドウマップ用（レンダーテクスチャ）
	static std::unique_ptr<DX::RenderTexture> m_shadowMapRT[4];
	// シャドウマップ用（デプスステンシル）
	static std::unique_ptr<DepthStencil> m_shadowMapDS;
};
