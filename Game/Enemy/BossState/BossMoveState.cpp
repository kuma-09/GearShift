#include "pch.h"
#include "BossMoveState.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Enemy/BossEnemy.h"

// コンストラクタ
BossMoveState::BossMoveState(Enemy* enemy)
{
	m_enemy = enemy;
}

// 初期化処理
void BossMoveState::Initialize()
{
	m_totalTime = 0;
}

// 更新処理
void BossMoveState::Update(float elapsedTime)
{

	m_totalTime += elapsedTime;

	// プレイヤーからの距離
	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	// 移動方向の変更
	if (m_totalTime < MOVE_INTERVAL) return;

	// 距離が離れたら近づきながらミサイル発射
	if (dis > MISSILE_DISTANCE)
	{
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
		Initialize();
		m_enemy->ChangeState(static_cast<BossEnemy*>(m_enemy)->GetMissileState());
		m_totalTime = 0;
	}
	else
	{
		RandomTackleOrGatlingState();
	}
}

// 描画処理
void BossMoveState::Render()
{
}

// 終了処理
void BossMoveState::Finalize()
{
}

/// <summary>
/// タックルかガトリング状態にランダムで遷移
/// </summary>
void BossMoveState::RandomTackleOrGatlingState()
{
	switch (rand() % 2)
	{
	case 0:
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * SPEED,0,0 });
		m_enemy->ChangeState(m_enemy->GetAttackState());
		m_totalTime = 0;
		break;
	case 1:
		m_enemy->ChangeState(static_cast<BossEnemy*>(m_enemy)->GetTackleState());
		m_totalTime = 0;
	default:
		break;
	}
}
