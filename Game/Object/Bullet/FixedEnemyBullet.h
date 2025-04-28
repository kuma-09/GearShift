#pragma once
#include "Bullet.h"

class Player;

/// <summary>
/// 敵が撃つ通常弾
/// </summary>
class FixedEnemyBullet :public Bullet
{
public:

	// コンストラクタ
	FixedEnemyBullet(Scene* scene, Collider::TypeID id);
	// デストラクタ
	~FixedEnemyBullet();
	// 初期化処理
	void Initialize(GameObject* object);
	// 弾を発射
	void Shot(GameObject* target);
	// 弾が何かに当たった時の処理
	void Hit();
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// 当たり判定の処理
	void Collision(Collider* collider);

private:
	// 進む速度　
	const float SPEED = 2.5f;
};