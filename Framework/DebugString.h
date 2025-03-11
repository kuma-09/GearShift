#pragma once
#include <SpriteFont.h>
#include <vector>
#include <string>

/// <summary>
/// デバッグ文字表示クラス
/// </summary>
class DebugString
{
private:
	// データメンバ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::vector<std::string> m_strings;
	float m_fontHeight;
	DirectX::SimpleMath::Color m_color;


public:
	// コンストラクタ
	DebugString(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		const wchar_t* fontFilePath
	);

	// デストラクタ
	~DebugString() = default;

	// 描画する文字列を登録する
	void AddString(const char* format, ...);

	// デバッグ文字列を描画する
	void Render(DirectX::CommonStates* states);

	// 描画する文字列の色を指定する
	void SetColor(DirectX::FXMVECTOR color = DirectX::Colors::White)
	{
		m_color = color;
	}
};
