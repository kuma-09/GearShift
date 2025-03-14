#pragma once
#include "Enemy.h"

class State;
class Bullet;
class EnemyAttackState;
class EnemyMoveState;

/// <summary>
/// 空中に浮くエネミー
/// </summary>
class HomingEnemy : public Enemy
{
public:
	// コンストラクタ
	HomingEnemy(IScene* scene, GameObject* target);
	// デストラクタ
	~HomingEnemy();
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
	void ChangeState(State* state);
	// 当たり判定の処理
	void Collision(Collider* collider);
private:
	// HPが残っているかチェック
	void CheckHP();

private:
	// 弾の最大数
	const int BULLET_COUNT = 10;
	// 弾配列
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	// ステート
	State* m_state;
};

