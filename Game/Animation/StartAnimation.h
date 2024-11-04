#pragma once

class StartAnimation
{
public:
	StartAnimation();
	~StartAnimation();
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


	std::string m_score;
	std::string m_mapNum;

	RECT m_size;

	// シーンチェンジフラグ
	bool m_isChangeScene;

	float m_height;
	float m_width;

	float m_posX;
	float m_posY;

	float m_postPosX;
	float m_postAlpha;

	float m_count;

	float m_startScale;
};

