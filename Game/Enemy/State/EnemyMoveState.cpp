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

void EnemyMoveState::Initialize(Enemy* enemy)
{
	m_enemy = enemy;
	m_totalTime = 0;
}

void EnemyMoveState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	if (m_totalTime >= 2)
	{
		if (dis > 40)
		{
			m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
			Initialize(m_enemy);
		}
		else
		{
			m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * 0.1f,0,0 });
			m_enemy->ChangeState(m_enemy->GetAttackState());
		}
	}
}

void EnemyMoveState::Render()
{
}

void EnemyMoveState::Finalize()
{
}
