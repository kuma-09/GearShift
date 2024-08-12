#include "pch.h"
#include "EnemyMoveState.h"
#include "EnemyAttackState.h"
#include "Game/Enemy/Enemy.h"

EnemyMoveState::EnemyMoveState(Enemy* enemy)
{
	m_enemy = enemy;
}

void EnemyMoveState::Initialize()
{

	m_totalTime = 0;
}

void EnemyMoveState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	m_enemy->SetVelocity({ 0,0,SPEED });

	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	if (dis > 20)
	{
		m_enemy->ChangeState(m_enemy->GetAttackState());
	}
}

void EnemyMoveState::Render()
{
}

void EnemyMoveState::Finalize()
{
}
