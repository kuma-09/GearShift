#pragma once

class Number
{
public:
	Number();
	~Number();
	void Initialize(DirectX::SimpleMath::Vector2 pos);
	void Render();
	void RenderTime();
	void RenderSlash();
	void SetNum(int num);
	void SetMaxNum(int num);

private:
	int m_number;
	int m_maxNumber;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;

	//”š‚ÌƒTƒCƒY
	RECT m_size;

	// ”š“¯m‚ÌŠÔŠu
	const int m_space = 35;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numberTexture;

	float m_alpha;
	int m_maxDigit;
	int m_digit;
};