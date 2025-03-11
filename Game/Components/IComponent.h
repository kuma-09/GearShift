#pragma once

class GameObject;

/// <summary>
/// GameObject�N���X�ɒǉ�����R���|�[�l���g�̊��N���X
/// </summary>
class IComponent
{
public:
	// �R���X�g���N�^
	IComponent() {};
	// �f�X�g���N�^
	virtual ~IComponent() = default;

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �e���w��
	void SetOwner(GameObject* a_pObject){ m_pOwner = a_pObject;}

	// �e���擾
	GameObject* GetOwner() { return m_pOwner; }

private:

	// �e
	GameObject* m_pOwner = nullptr;
};
