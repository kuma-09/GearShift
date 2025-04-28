#pragma once
#include "Bullet.h"

class Player;

/// <summary>
/// プレイヤーが使用する通常弾
/// </summary>
class NormalBullet :public Bullet
{
public:
	// コンストラクタ
	NormalBullet(Scene* scene, Collider::TypeID id);
	// デストラクタ
	~NormalBullet();
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

	// 生存時間
	const float MAX_TIME = 10.0f;
	// 経過時間
	float m_totalTime;
	// 進む速度
	const float SPEED = 100.0f;

};