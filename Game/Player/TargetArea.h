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
	bool Update(Player* plaer, GameObject* target);
	void Render(GameObject* target);
	void Finalize();

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() { return m_scale; }

private:

	DirectX::SimpleMath::Vector2 GetScreenPosition(GameObject* target);

	Graphics* m_graphics;

	// ターゲット範囲
	float m_range;
	float m_aspect;

	const DirectX::SimpleMath::Vector2 m_textureSize = { 500 , 500 };

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// カメラポジション
	DirectX::SimpleMath::Vector3 m_eyePosition;

	// 注視点
	DirectX::SimpleMath::Vector3 m_targetPosition;

	// 画面の中心用の画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	const float SCALE = 1.0f;
	float m_scale;
	float m_rotate;

	DirectX::SimpleMath::Vector2 m_targetScreenPos;

	float lerp(float start, float end, float t) {
		return (1 - t) * start + t * end;
	}
};

