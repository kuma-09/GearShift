#include "pch.h"
#include "EnemyAttackState.h"
#include "EnemyMoveState.h"
#include "Game/Enemy/Enemy.h"

// �R���X�g���N�^
EnemyAttackState::EnemyAttackState(Enemy* enemy)
{
	m_totalTime = 0;
	m_enemy = enemy;
}

// ����������
void EnemyAttackState::Initialize()
{

}

// �X�V����
void EnemyAttackState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	// ���Ԃ��o�߂�����e�𔭎�
	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_enemy->Shot();
		m_totalTime = 0;
		// ���˂�����ړ��X�e�[�g�ɑJ��
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}

}

// �`�揈��
void EnemyAttackState::Render()
{
}

// �I������
void EnemyAttackState::Finalize()
{
}
