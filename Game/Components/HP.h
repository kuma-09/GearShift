#pragma once
#include "Component.h"

/// <summary>
/// HP�R���|�[�l���g�N���X
/// </summary>
class HP : public Component
{
public:
	// �R���X�g���N�^
	HP();
	// �f�X�g���N�^
	~HP();
	// ����������
	void Initialize(int hp);
	// �X�V����
	void Update(float elapsedTime);

	// HP���㏑��
	void SetHP(int hp) { m_hp = hp; }
	// ���݂�HP���擾
	int  GetHP() { return m_hp; }

private:
	// HP�ۑ��ϐ�
	int m_hp;
};

