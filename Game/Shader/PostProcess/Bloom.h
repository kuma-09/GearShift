#pragma once
#include <Framework/Graphics.h>
#include <Framework/RenderTexture.h>

class Bloom
{
public:
	static void Initialize();
	static void Update(float elapsedTime);

	// ブルーム適用用のレンダーターゲットに変更
	static void BeginBloom();
	static void EndBloom(ID3D11ShaderResourceView* srv);

	static void BloomTextureShow();
private:

	static Graphics* m_graphics;

	static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;

	// 頂点情報
	static DirectX::VertexPositionTexture m_vertex[4];

	// ポストプロセス用
	// レンダーテクスチャ(シーン全体)
	static std::unique_ptr<DX::RenderTexture> m_offscreenRT_Bloom;

	// レンダーテクスチャ(ブルーム用)
	static std::unique_ptr<DX::RenderTexture> m_blur1RT;
	static std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// プリミティブバッチ
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// ベーシックポストプロセス
	static std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// デュアルポストプロセス
	static std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

	static const float PIXELSIZE;
};
