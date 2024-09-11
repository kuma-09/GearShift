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
	float m_range;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// 画面の中心用の画像
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

