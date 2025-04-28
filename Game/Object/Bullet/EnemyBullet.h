#pragma once
#include "Bullet.h"

class Player;

/// <summary>
/// 敵が撃つ通常弾(予測射撃)
/// </summary>
class EnemyBullet :public Bullet
{
public:
	// コンストラクタ
	EnemyBullet(Scene* scene, Collider::TypeID id);
	// デストラクタ
	~EnemyBullet();
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
	DirectX::SimpleMath::Vector3 LinePrediction(Player* target);
	float PlusMin(float a, float b);

private:

	const float SPEED = 1.5f;
	const float DIFFUSION = 1.0f;

};