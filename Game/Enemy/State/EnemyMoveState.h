#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// 敵の移動ステート
/// </summary>
class EnemyMoveState : public State
{
public:
	// コンストラクタ
	EnemyMoveState(Enemy* enemy);
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

	// 移動速度
	const float SPEED = 0.1f;
	// 移動ステートのインターバル
	const float MOVE_INTERVAL = 3.0f;
	// 攻撃ステート変更距離
	const float SHOT_DISTANCE = 40.0f;
	// ステートの経過時間
	float m_totalTime;
};