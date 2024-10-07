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
	bool Update(Player* plaer, GameObject* target);
	void Render();
	void Finalize();

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() { return m_scale; }

private:

	Graphics* m_graphics;

	// �^�[�Q�b�g�͈�
	const float m_range = 200;

	const DirectX::SimpleMath::Vector2 m_textureSize = { 500 , 500 };

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �J�����|�W�V����
	DirectX::SimpleMath::Vector3 m_eyePosition;

	// �����_
	DirectX::SimpleMath::Vector3 m_targetPosition;

	// ��ʂ̒��S�p�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	float m_scale;
};

