#pragma once

#include "Game/UI/UI.h"

class FinishAnimation
{
public:
	FinishAnimation();
	~FinishAnimation();
	void Initialize();
	bool Update(float elapsedTime);
	void Render();
private:
	int m_targetNumber;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_GoTexture;

	std::unique_ptr<UI> m_bar;
	std::unique_ptr<UI> m_font;

	std::string m_score;
	std::string m_mapNum;

	RECT m_size;

	// シーンチェンジフラグ
	bool m_isChangeScene;

	float m_nowTime;
	float m_targetTime;
	float m_result;
	float m_textResult;
};

