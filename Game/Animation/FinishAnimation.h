#pragma once

#include "Game/UI/UI.h"

/// <summary>
/// ゲームクリア時のアニメーションクラス
/// </summary>
class FinishAnimation
{
public:
	// コンストラクタ
	FinishAnimation();
	// デストラクタ
	~FinishAnimation();
	// 初期化処理
	void Initialize();
	// 更新処理
	bool Update(float elapsedTime);
	// 描画処理
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

	// シーンチェンジフラグ
	bool m_isChangeScene;

	float m_nowTime;
	float m_targetTime;
	float m_result;

};

