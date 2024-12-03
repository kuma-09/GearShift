#include "pch.h"
#include "BossMissileState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"


BossMissileState::BossMissileState(Enemy* enemy)
{
	m_enemy = enemy;
	m_totalTime = 0;
}

void BossMissileState::Initialize()
{
}

void BossMissileState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	if (m_totalTime >= SHOT_INTERVAL)
	{
		static_cast<BossEnemy*>(m_enemy)->ShotMissile();
		m_totalTime = 0;
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

void BossMissileState::Render()
{
}

void BossMissileState::Finalize()
{
}
