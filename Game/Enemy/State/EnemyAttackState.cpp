#include "pch.h"
#include "EnemyAttackState.h"
#include "EnemyMoveState.h"
#include "Game/Enemy/Enemy.h"

EnemyAttackState::EnemyAttackState(Enemy* enemy)
{
	m_totalTime = 0;
	m_enemy = enemy;
}

void EnemyAttackState::Initialize()
{

}

void EnemyAttackState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	m_enemy->SetVelocity({0,0,0 });
	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_enemy->Shot();
		m_totalTime = 0;
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}

}

void EnemyAttackState::Render()
{
}

void EnemyAttackState::Finalize()
{
}
