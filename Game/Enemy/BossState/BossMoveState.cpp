#include "pch.h"
#include "BossMoveState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"

BossMoveState::BossMoveState(Enemy* enemy)
{
	m_enemy = enemy;
}

void BossMoveState::Initialize()
{
	m_totalTime = 0;
}

void BossMoveState::Update(float elapsedTime)
{

	m_totalTime += elapsedTime;

	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	if (m_totalTime >= 2)
	{
		if (dis > 80)
		{
			m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
			Initialize();
			m_enemy->ChangeState(static_cast<BossEnemy*>(m_enemy)->GetMissileState());
			m_totalTime = 0;
		}
		else
		{
			switch (rand() % 2)
			{
			case 0:
				m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * SPEED,0,0 });
				m_enemy->ChangeState(m_enemy->GetAttackState());
				m_totalTime = 0;
				break;
			case 1:
				m_enemy->ChangeState(static_cast<BossEnemy*>(m_enemy)->GetTackleState());
				m_totalTime = 0;
			default:
				break;
			}
		}
	}
}

void BossMoveState::Render()
{
}

void BossMoveState::Finalize()
{
}
