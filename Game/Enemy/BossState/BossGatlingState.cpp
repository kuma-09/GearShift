#include "pch.h"
#include "BossGatlingState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"


BossGatlingState::BossGatlingState(Enemy* enemy)
{
	m_enemy = enemy;
	m_totalTime = 0;
	m_bulletInterval = 0;
}

void BossGatlingState::Initialize()
{
	static_cast<BossEnemy*>(m_enemy)->ReloadGatling();
	m_totalTime = 0;
	m_bulletInterval = 0;
}

void BossGatlingState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	m_bulletInterval += elapsedTime;
	if (m_bulletInterval >= SHOT_INTERVAL)
	{
		static_cast<BossEnemy*>(m_enemy)->ShotGatling();
		m_bulletInterval = 0;
	}
	if (m_totalTime >= STATE_INTERVAL)
	{
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

void BossGatlingState::Render()
{
}

void BossGatlingState::Finalize()
{
}
