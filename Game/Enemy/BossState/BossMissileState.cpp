#include "pch.h"
#include "BossMissileState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"

// �R���X�g���N�^
BossMissileState::BossMissileState(Enemy* enemy)
{
	m_enemy = enemy;
	m_totalTime = 0;
}

// ����������
void BossMissileState::Initialize()
{
	m_totalTime = 0;
}

// �X�V����
void BossMissileState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	// �~�T�C���𔭎�
	if (m_totalTime >= SHOT_INTERVAL)
	{
		static_cast<BossEnemy*>(m_enemy)->ShotMissile();
		m_totalTime = 0;

		// �ړ��X�e�[�g�ɑJ��
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

// �`�揈��
void BossMissileState::Render()
{
}

// �I������
void BossMissileState::Finalize()
{
}
