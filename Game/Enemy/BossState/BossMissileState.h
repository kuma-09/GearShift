#pragma once
#include "Game/IState.h"

class Enemy;

/// <summary>
/// ボスのミサイル発射ステート
/// </summary>
class BossMissileState : public IState
{
public:
	// コンスタント
	BossMissileState(Enemy* enemy);
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

	// ミサイル発射間隔
	const float SHOT_INTERVAL = 0.3f;
	// ステートの経過時間
	float m_totalTime;
};
