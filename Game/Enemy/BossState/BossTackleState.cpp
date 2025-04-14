#include "pch.h"
#include "BossTackleState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"
#include "Framework/Easing.h"

// コンストラクタ
BossTackleState::BossTackleState(Enemy* enemy)
{
	m_enemy = enemy;
}

// 初期化処理
void BossTackleState::Initialize()
{
	m_totalTime = 0;
}

// 更新処理
void BossTackleState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0, Easing::InCubic(m_totalTime,STATE_INTERVAL) * -SPEED});
	if (m_totalTime >= STATE_INTERVAL)
	{
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

// 描画処理
void BossTackleState::Render()
{
}

// 終了処理
void BossTackleState::Finalize()
{
}
