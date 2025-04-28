#pragma once
#include "Component.h"

class Collider;

/// <summary>
/// �d�̓R���|�[�l���g�N���X
/// </summary>
class Physics : public Component
{
public:

	// �R���X�g���N�^
	Physics();
	// �f�X�g���N�^
	~Physics();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);

	// �d�͂����Z�b�g����֐�
	void Reset() { m_gravity = 0; }

private:
	// �d�͂̏���l
	const float MAX_GRAVITY = 0.5f;
	// �d�͒l
	float m_gravity;
	// �n�ʂɐG��Ă��邩
	bool m_onFloor;
};

