#include "pch.h"
#include "EnemyMoveState.h"
#include "Game/Enemy/Enemy.h"

EnemyMoveState::EnemyMoveState(Enemy* enemy)
{
	m_enemy = enemy;
}

void EnemyMoveState::Initialize()
{
		m_enemy->SetVelocity({ 0,0,SPEED });
}

void EnemyMoveState::Update(float elapsedTime)
{

}

void EnemyMoveState::Render()
{
}

void EnemyMoveState::Finalize()
{
}
