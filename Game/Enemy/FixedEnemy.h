#pragma once
#include "Enemy.h"

class IState;
class FixedEnemyBullet;

/// <summary>
/// 固定エネミー
/// </summary>
class FixedEnemy : public Enemy
{
public:
	// コンストラクタ
	FixedEnemy(Scene* scene, GameObject* target);
	// デストラクタ
	~FixedEnemy();
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 終了処理
	void Finalize();

	// 弾を発射
	void Shot();
	// ステートを変更
	void ChangeState(IState* state);
	// 当たり判定の処理
	void Collision(Collider* collider);

private:
	void CheckHP();
private:
	// 弾配列
	std::unique_ptr<FixedEnemyBullet> m_bullet;
	// ステート
	IState* m_state;
};

