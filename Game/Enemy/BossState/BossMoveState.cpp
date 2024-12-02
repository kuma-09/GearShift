#include "pch.h"
#include "BossMoveState.h"
#include "Game/Enemy/Enemy.h"

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
		if (dis > 40)
		{
			m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
			Initialize();
		}
		else
		{
			m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * 0.1f,0,0 });
			m_enemy->ChangeState(m_enemy->GetAttackState());
		}
	}
}

void BossMoveState::Render()
{
}

void BossMoveState::Finalize()
{
}
