#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// 敵の攻撃ステート
/// </summary>
class EnemyAttackState : public State
{
public:
	// コンストラクタ
	EnemyAttackState(Enemy* enemy);
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();
private:
	Enemy* m_enemy;

	// 攻撃の間隔
	const float SHOT_INTERVAL = 0.3f;
	// ステートの経過時間
	float m_totalTime;
};
