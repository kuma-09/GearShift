#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// ボスエネミー移動ステート
/// </summary>
class BossMoveState : public State
{
public:
	// コンストラクタ
	BossMoveState(Enemy* enemy);
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();
private:
	// タックルかガトリング状態にランダムで遷移
	void RandomTackleOrGatlingState();
private:
	Enemy* m_enemy;

	// 移動スピード
	const float SPEED = 0.2f;
	// 移動方向の変更間隔
	const float MOVE_INTERVAL = 2.0f;
	// ミサイル発射ステート遷移距離
	const float MISSILE_DISTANCE = 80.0f;
	// 移動ステートの経過時間
	float m_totalTime;
};