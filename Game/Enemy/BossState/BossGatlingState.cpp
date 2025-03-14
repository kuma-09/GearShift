#include "pch.h"
#include "BossGatlingState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"

// コンストラクタ
BossGatlingState::BossGatlingState(Enemy* enemy)
{
	m_enemy = enemy;
	m_totalTime = 0;
	m_bulletInterval = 0;
}

// 初期化処理
void BossGatlingState::Initialize()
{
	static_cast<BossEnemy*>(m_enemy)->ReloadGatling();
	m_totalTime = 0;
	m_bulletInterval = 0;
}

// 更新処理
void BossGatlingState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
	m_bulletInterval += elapsedTime;

	// 次の弾を発射
	if (m_bulletInterval >= SHOT_INTERVAL)
	{
		static_cast<BossEnemy*>(m_enemy)->Shot();
		m_bulletInterval = 0;
	}

	// 移動ステートに遷移
	if (m_totalTime >= STATE_INTERVAL)
	{
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}
}

// 描画処理
void BossGatlingState::Render()
{
}

// 終了処理
void BossGatlingState::Finalize()
{
}
