#pragma once
#include <Framework/Graphics.h>
#include <Framework/RenderTexture.h>

class PostProcess
{
public:
	PostProcess();
	~PostProcess();

	void Initialize();
	void Update(float elapsedTime);

	// 通常の描画レンダーターゲットに変更
	void BeginNormal();

	// ブルーム適用用のレンダーターゲットに変更
	void BeginBloom();

	// レンダーテクスチャを合成
	void combinationRT();

	// 最終結果にノイズを適用
	void ApplyNoise(ID3D11DeviceContext* context);

	void SetNoise(bool isNoise) { m_isNoise = isNoise; }
	void SetDamage(bool isDamage) { m_isDamage = isDamage; }
	void SetBoost(bool isBoost) { m_isBoost = isBoost; }

private:

	Graphics* m_graphics;

	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 定数バッファ
	struct ConstantBuffer
	{
		float time;
		float pad[3];
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// 頂点情報
	DirectX::VertexPositionTexture m_vertex[4];

	// ポストプロセス用
	// レンダーテクスチャ(シーン全体)
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;
	std::unique_ptr<DX::RenderTexture> m_offscreenRT_Bloom;
	std::unique_ptr<DX::RenderTexture> m_offscreenRT_Normal;
	std::unique_ptr<DX::RenderTexture> m_finalRenderTexture;

	// レンダーテクスチャ(ブルーム用)
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// ベーシックポストプロセス
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// デュアルポストプロセス
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

	// 通常時のシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
	// ミッション開始時のノイズシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_startVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_startPS;
	// 被弾時のノイズシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_noiseVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_noisePS;
	
	bool m_isStartNoise;
	bool m_isNoise;
	bool m_isDamage;
	bool m_isBoost;
	float m_nowTime;
	const float PIXELSIZE = 16.0f;
	const float m_maxStartNoiseTime = 1.0f;
	const float m_maxNoiseTime = 0.25f;
	const float m_maxDamageNoiseTime = 0.25f;
	const float m_maxBoostBlur = 0.5f;
};
