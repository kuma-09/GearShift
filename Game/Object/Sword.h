#pragma once
#include "Game/GameObject.h"

/// <summary>
/// プレイヤー攻撃用のブレードクラス
/// </summary>
class Blade :public GameObject
{
public:

	// ブレードの状態
	enum BladeState
	{
		UNUSED,
		USING,
		USED,
	};

	// コンストラクタ
	Blade(IScene* scene, Collider::TypeID id);
	// デストラクタ
	~Blade();
	// 初期化処理
	void Initalize(GameObject* object);
	// 弾を発射
	void Shot(GameObject* object);
	// 何かに当たった時の処理
	void Hit();
	// 更新処理
	void Update(float elapsedTime);

	// ブレードの状態を取得
	bool GetIsHit() { return m_isHit; }
	BladeState GetState() { return m_state; }
	void SetState(BladeState state) { m_state = state; }

	// 当たり判定の処理
	void Collision(Collider* collider);

private:
	// ステート
	BladeState m_state;
	// 親
	GameObject* m_owner;

	bool m_isHit;
	float m_rotate;
	const DirectX::SimpleMath::Vector3 SIZE = {5.f,5.f,5.f};
};