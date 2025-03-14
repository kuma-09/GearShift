#include "pch.h"
#include "EnemyMoveState.h"
#include "EnemyAttackState.h"
#include "Game/Enemy/Enemy.h"

// コンストラクタ
EnemyMoveState::EnemyMoveState(Enemy* enemy)
	:
	m_totalTime{}
{
	m_enemy = enemy;
}

// 初期化処理
void EnemyMoveState::Initialize()
{
	m_totalTime = 0;
}

// 更新処理
void EnemyMoveState::Update(float elapsedTime)
{

	m_totalTime += elapsedTime;

	// 行動変更インターバルチェック
	if (m_totalTime < MOVE_INTERVAL) return;

	// プレイヤーとの距離を計算
	float dis = (m_enemy->GetPosition() - m_enemy->GetTarget()->GetPosition()).Length();

	
	if (dis > SHOT_DISTANCE)
	{
		// 遠ければプレイヤーに近づく
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ 0,0,-SPEED });
		Initialize();
	}
	else
	{
		// 近ければ攻撃ステートに遷移
		m_enemy->SetVelocity(DirectX::SimpleMath::Vector3{ float(rand() % 2 * 2 - 1) * 0.1f,0,0 });
		m_enemy->ChangeState(m_enemy->GetAttackState());
	}
}

// 描画処理
void EnemyMoveState::Render()
{
}

// 終了処理
void EnemyMoveState::Finalize()
{
}
