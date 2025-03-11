#pragma once

#include "Game/UI/UI.h"

/// <summary>
/// �Q�[���N���A���̃A�j���[�V�����N���X
/// </summary>
class FinishAnimation
{
public:
	// �R���X�g���N�^
	FinishAnimation();
	// �f�X�g���N�^
	~FinishAnimation();
	// ����������
	void Initialize();
	// �X�V����
	bool Update(float elapsedTime);
	// �`�揈��
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

	std::unique_ptr<UI> m_bar;
	std::unique_ptr<UI> m_font;

	// �V�[���`�F���W�t���O
	bool m_isChangeScene;

	float m_nowTime;
	float m_targetTime;
	float m_result;

};

