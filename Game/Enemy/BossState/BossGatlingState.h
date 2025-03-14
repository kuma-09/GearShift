#pragma once
#include "Game/State.h"

class Enemy;

/// <summary>
/// ボスの弾連射ステート
/// </summary>
class BossGatlingState : public State
{
public:
	BossGatlingState(Enemy* enemy);
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

	// 弾連射ステートの時間
	const float STATE_INTERVAL = 1.0f;
	// 弾の発射間隔
	const float SHOT_INTERVAL = 0.1f;
	// 弾のInterval時間
	float m_bulletInterval;
	// ステートの経過時間
	float m_totalTime;
};
