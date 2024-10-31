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
	void BeginNormal();
	void BeginBloom();
	void combinationRT();
	void SetNoise(bool isNoise) { m_isNoise = isNoise; }

private:

	Graphics* m_graphics;

	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// ポストプロセス用
	// レンダーテクスチャ(シーン全体)
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;
	std::unique_ptr<DX::RenderTexture> m_offscreenRT_Bloom;
	std::unique_ptr<DX::RenderTexture> m_offscreenRT_Normal;
	std::unique_ptr<DX::RenderTexture> m_finalRenderTexture;

	// レンダーテクスチャ(ブルーム用)
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// ベーシックポストプロセス
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// デュアルポストプロセス
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_noisePS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_noiseVS;

	bool m_isNoise;
	float m_nowTime;
	const float m_maxNoiseTime = 1.0f;

};
