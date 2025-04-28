#pragma once
#include "Component.h"
#include "Framework/InputManager.h"

/// <summary>
/// ���̓R���|�[�l���g�N���X
/// </summary>
class InputComponent : public Component
{
public:
	// �R���X�g���N�^
	InputComponent();
	// �f�X�g���N�^
	~InputComponent();
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// ���͕������擾
	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

private:
	InputManager* m_inputManager;

	// ���͕����ۑ��p
	DirectX::SimpleMath::Vector3 m_velocity;

	// �ő呬�x
	const float MAX_SPEED = 0.95f;
};

