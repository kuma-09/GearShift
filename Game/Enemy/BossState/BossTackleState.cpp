#include "pch.h"
#include "BossTackleState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"
#include "Framework/Easing.h"

BossTackleState::BossTackleState(Enemy* enemy)
{
	m_enemy = enemy;
}

void BossTackleState::Initialize()
{
	m_totalTime = 0;
}

void BossTackleState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();
	m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0, Easing::InCubic(m_totalTime,STATE_INTERVAL) * -SPEED});
	if (m_totalTime >= STATE_INTERVAL)
	{
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

void BossTackleState::Render()
{
}

void BossTackleState::Finalize()
{
}
