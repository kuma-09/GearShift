#pragma once
#include "Game/IState.h"

class Enemy;

/// <summary>
/// 動かない敵の攻撃ステート
/// </summary>
class FixedAttackState : public IState
{
public:
	// コンストラクタ
	FixedAttackState(Enemy* enemy);
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

	// 弾を発射する間隔
	const float SHOT_INTERVAL = 2.0f;
	// 経過時間
	float m_totalTime;
};
