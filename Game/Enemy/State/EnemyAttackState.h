#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// �G�̍U���X�e�[�g
/// </summary>
class EnemyAttackState : public State
{
public:
	// �R���X�g���N�^
	EnemyAttackState(Enemy* enemy);
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

	// �U���̊Ԋu
	const float SHOT_INTERVAL = 0.3f;
	// �X�e�[�g�̌o�ߎ���
	float m_totalTime;
};
