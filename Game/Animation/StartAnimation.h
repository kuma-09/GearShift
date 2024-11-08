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
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_GoTexture;


	std::string m_score;
	std::string m_mapNum;

	RECT m_size;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;

	float m_nowTime;
	float m_targetTime;
	float m_result;
	float m_textResult;
};

