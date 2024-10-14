#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Object/SkyDome.h"
#include "Game/Title/TitlePlayer.h"
#include "Game/Title/TitleCamera.h"
#include "Game/Object/Floor.h"
#include "UI/UI.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"

class TitleScene final: public IScene
{
public:

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<TitleCamera> m_camera;
	std::unique_ptr<SkyDome> m_skydome;
	std::unique_ptr<TitlePlayer> m_player;
	DirectX::Model* m_floorModel;
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;
	std::unique_ptr<UI> m_titleLogo;
	
private:

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// 定数バッファの構造体
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR lightPosition;	// ライトの位置
		DirectX::XMVECTOR lightDirection;	// ライトの方向
		DirectX::XMVECTOR lightAmbient;		// ライトの環境光
	};

	// 定数バッファの構造体
	struct ConstantBuffer2
	{
		float fCosTheta;		// スポットライトのfov/2
		float pad[3];
	};

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer2;

	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;


	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE = 1024;

	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// シャドウマップ用（デプスステンシル）
	std::unique_ptr<DepthStencil> m_shadowMapDS;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Tex;

	// ライトの位置
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ライトの回転
	DirectX::SimpleMath::Quaternion m_lightRotate;

	// サンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// スポットライトの範囲の角度
	float m_lightTheta;

};

