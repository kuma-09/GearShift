#include "pch.h"
#include "FixedAttackState.h"
#include "Game/Enemy/Enemy.h"

FixedAttackState::FixedAttackState(Enemy* enemy)
{
	m_totalTime = 0;
	m_enemy = enemy;
}

void FixedAttackState::Initialize()
{

}

void FixedAttackState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;


	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_enemy->Shot();
		m_totalTime = 0;
	}

}

void FixedAttackState::Render()
{
}

void FixedAttackState::Finalize()
{
}
