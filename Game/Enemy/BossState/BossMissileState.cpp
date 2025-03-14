#include "pch.h"
#include "BossMissileState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"

// コンストラクタ
BossMissileState::BossMissileState(Enemy* enemy)
{
	m_enemy = enemy;
	m_totalTime = 0;
}

// 初期化処理
void BossMissileState::Initialize()
{
	m_totalTime = 0;
}

// 更新処理
void BossMissileState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	// ミサイルを発射
	if (m_totalTime >= SHOT_INTERVAL)
	{
		static_cast<BossEnemy*>(m_enemy)->ShotMissile();
		m_totalTime = 0;

		// 移動ステートに遷移
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

// 描画処理
void BossMissileState::Render()
{
}

// 終了処理
void BossMissileState::Finalize()
{
}
