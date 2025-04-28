#pragma once
#include "Game/IState.h"

class Enemy;

/// <summary>
/// �����Ȃ��G�̍U���X�e�[�g
/// </summary>
class FixedAttackState : public IState
{
public:
	// �R���X�g���N�^
	FixedAttackState(Enemy* enemy);
	// ����������
	void Initialize();
	// �X�V����
	void Update(float elapsedTime);
	// �`�揈��
	void Render();
	// �I������
	void Finalize();
private:
	Enemy* m_enemy;

	// �e�𔭎˂���Ԋu
	const float SHOT_INTERVAL = 2.0f;
	// �o�ߎ���
	float m_totalTime;
};
