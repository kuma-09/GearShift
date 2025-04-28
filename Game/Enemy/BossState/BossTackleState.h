#pragma once
#include "Game/IState.h"

class Enemy;

/// <summary>
/// ボスの突撃ステート
/// </summary>
class BossTackleState : public IState
{
public:
	// コンストラクタ
	BossTackleState(Enemy* enemy);
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

	// ステートの時間
	const float STATE_INTERVAL = 1.0f;
	// 移動スピード
	const float SPEED = 0.3f;
	// ステートの経過時間
	float m_totalTime;
};
