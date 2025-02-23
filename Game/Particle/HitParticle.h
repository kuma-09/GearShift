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

	Graphics* m_graphics;

	DirectX::VertexPositionColorTexture    m_vertices[4];

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
