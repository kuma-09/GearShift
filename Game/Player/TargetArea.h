#pragma once
#include "Framework/Graphics.h"
#include "Game/GameObject.h"

class Player;

class TargetArea
{
public:
	TargetArea();
	~TargetArea();

	void Initialize();
	bool Update(Player* plaer ,GameObject* target);
	void Render();
	void Finalize();

private:

	Graphics* m_graphics;

	// ターゲット範囲
	const float m_range = 200;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// カメラポジション
	DirectX::SimpleMath::Vector3 m_eyePosition;

	// 注視点
	DirectX::SimpleMath::Vector3 m_targetPosition;

	// 画面の中心用の画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

