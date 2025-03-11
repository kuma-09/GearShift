#pragma once

#include "Game/UI/UI.h"

/// <summary>
/// ゲーム開始時のアニメーションクラス
/// </summary>
class StartAnimation
{
public:
	// コンストラクタ
	StartAnimation();
	// デストラクタ
	~StartAnimation();
	// 初期化処理
	void Initialize();
	// 更新処理
	bool Update(float elapsedTime);
	// 描画処理
	void Render();
private:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_GoTexture;

	std::unique_ptr<UI> m_bar;
	std::unique_ptr<UI> m_font;

	// シーンチェンジフラグ
	bool m_isChangeScene;

	float m_nowTime;
	float m_targetTime;
	float m_result;
};

