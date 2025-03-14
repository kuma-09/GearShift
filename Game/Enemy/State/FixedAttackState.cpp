#include "pch.h"
#include "FixedAttackState.h"
#include "Game/Enemy/Enemy.h"

// コンストラクタ
FixedAttackState::FixedAttackState(Enemy* enemy)
	:
	m_totalTime{}
{
	m_enemy = enemy;
}

// 初期化処理
void FixedAttackState::Initialize()
{
	m_totalTime = 0;
}

// 更新処理
void FixedAttackState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;

	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_enemy->Shot();
		Initialize();
	}
}

// 描画処理
void FixedAttackState::Render()
{
}

// 終了処理
void FixedAttackState::Finalize()
{
}
