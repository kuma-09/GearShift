#include "pch.h"
#include "EnemyAttackState.h"
#include "EnemyMoveState.h"
#include "Game/Enemy/Enemy.h"

// コンストラクタ
EnemyAttackState::EnemyAttackState(Enemy* enemy)
{
	m_totalTime = 0;
	m_enemy = enemy;
}

// 初期化処理
void EnemyAttackState::Initialize()
{

}

// 更新処理
void EnemyAttackState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	// 時間が経過したら弾を発射
	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_enemy->Shot();
		m_totalTime = 0;
		// 発射したら移動ステートに遷移
		m_enemy->ChangeState(m_enemy->GetMoveState());
	}

}

// 描画処理
void EnemyAttackState::Render()
{
}

// 終了処理
void EnemyAttackState::Finalize()
{
}
