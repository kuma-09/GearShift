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
	void Render(GameObject* target);
	void Finalize();

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() { return m_scale; }

private:

	DirectX::SimpleMath::Vector2 GetScreenPosition(GameObject* target);

	Graphics* m_graphics;

	// �^�[�Q�b�g�͈�
	float m_range;
	float m_aspect;

	const DirectX::SimpleMath::Vector2 m_textureSize = { 500 , 500 };

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// �J�����|�W�V����
	DirectX::SimpleMath::Vector3 m_eyePosition;

	// �����_
	DirectX::SimpleMath::Vector3 m_targetPosition;

	// ��ʂ̒��S�p�̉摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	const float SCALE = 1.0f;
	float m_scale;
	float m_rotate;

	DirectX::SimpleMath::Vector2 m_targetScreenPos;

	float lerp(float start, float end, float t) {
		return (1 - t) * start + t * end;
	}
};

