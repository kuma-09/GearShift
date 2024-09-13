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

	// �^�[�Q�b�g�͈�
	const float m_range = 200;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �J�����|�W�V����
	DirectX::SimpleMath::Vector3 m_eyePosition;

	// �����_
	DirectX::SimpleMath::Vector3 m_targetPosition;

	// ��ʂ̒��S�p�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

