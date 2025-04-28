#pragma once
#include "Component.h"
#include "Framework/InputManager.h"

/// <summary>
/// �ړ��𔽉f������R���|�[�l���g�N���X
/// </summary>
class Move : public Component
{
public:
	// �R���X�g���N�^
	Move();
	// �f�X�g���N�^
	~Move();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �ړ��ʂ��擾
	DirectX::SimpleMath::Vector3 GetVelocity();
	// �ړ����͂���Ă��邩
	bool GetIsMove() { return m_isMove; }

private:
	const float MAX_SPEED = 0.95f;

private:
	InputManager* m_inputManager;
	DirectX::SimpleMath::Vector3 m_velocity;
	// �ړ����̗͂L��
	bool m_isMove;


};

