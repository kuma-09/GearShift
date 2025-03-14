#include "pch.h"
#include "FixedAttackState.h"
#include "Game/Enemy/Enemy.h"

// �R���X�g���N�^
FixedAttackState::FixedAttackState(Enemy* enemy)
	:
	m_totalTime{}
{
	m_enemy = enemy;
}

// ����������
void FixedAttackState::Initialize()
{
	m_totalTime = 0;
}

// �X�V����
void FixedAttackState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_enemy->Shot();
		Initialize();
	}
}

// �`�揈��
void FixedAttackState::Render()
{
}

// �I������
void FixedAttackState::Finalize()
{
}
