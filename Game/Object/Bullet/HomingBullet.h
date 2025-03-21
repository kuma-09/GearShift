#pragma once
#include "Bullet.h"

/// <summary>
/// ターゲットを追尾する弾
/// </summary>
class HomingBullet :public Bullet
{
public:
	// コンストラクタ
	HomingBullet(IScene* scene, Collider::TypeID id);
	// デストラクタ
	~HomingBullet();	// 初期化処理
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
	const float SPEED = 2.0f;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_velocity;
	// 経過時間
	float m_period;
};