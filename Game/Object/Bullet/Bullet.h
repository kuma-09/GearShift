#pragma once
#include "Game/GameObject.h"

/// <summary>
/// 弾クラスの基底クラス
/// </summary>
class Bullet :public GameObject
{
public:

	// 弾の状態
	enum BulletState
	{
		UNUSED,
		FLYING,
		USED,
	};

	// コンストラクタ
	virtual ~Bullet() = default;
	// 初期化処理
	virtual void Initialize(GameObject* object) = 0;
	// 弾を発射
	virtual void Shot(GameObject* target) = 0;
	// 弾が何かに当たった時の処理
	virtual void Hit() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render() = 0;

	// ゲッターセッター
	//  弾の状態
	void SetState(BulletState state) { m_state = state; }
	BulletState GetState() { return m_state; }

	// ターゲット
	void SetTarget(GameObject* target) { m_target = target; }
	GameObject* GetTarget() { return m_target; }

	// 弾を発射した親
	GameObject* GetOwner() { return m_owner; }
	void SetOwner(GameObject* owner) { m_owner = owner; }

	// 攻撃時の値
	void SetAttackPoint(int attackPoint) { m_attackPoint = attackPoint; }
	int GetAttackPoint() { return m_attackPoint; }

	// 当たり判定の処理
	virtual void Collision(Collider* collider) = 0;

private:
	// 弾の状態
	BulletState m_state;
	// 弾が狙うターゲット
	GameObject* m_target;
	// 弾を発射した親
	GameObject* m_owner;
	// 攻撃力
	int m_attackPoint;
};