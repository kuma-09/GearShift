/*
	@file	HitParticle.h
	@brief	一般的なシーンクラス
*/
#pragma once
#include "Framework/Graphics.h"

// 一般的なシーンクラス
class HitParticle
{
private:

	// ベーシックエフェクト
	// テクスチャマッピング、頂点カラー、ライティング等をサポートする
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// プリミティブバッチ
	// 線や三角形などのジオメトリを描画するために必要
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// 入力レイアウト
	// 頂点情報の種類を指定する
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;


	// ★以下、追記する変数など★

	// 三角形で使用する変数
	DirectX::VertexPositionTexture    m_vertices[4];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;	// ブレンドステート

	float							ALPHA;
	float							RG;
	
	DirectX::SimpleMath::Vector3 m_pos;
	DirectX::SimpleMath::Vector3 m_vel;


public:
	HitParticle();
	~HitParticle();

	void Initialize(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, DirectX::SimpleMath::Vector2 size = {0.25f,0.25f});
	bool Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Finalize();

	float GetAlpha() { return ALPHA; }

	static int get_rand(int min_val, int max_val) {
		return (int)((rand() % (max_val + 1 - min_val)) + min_val);
	}
};
