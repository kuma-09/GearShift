#pragma once
#include "Component.h"
#include "Framework/Graphics.h"
#include "Game/Manager/CollisionManager.h"

/// <summary>
/// 当たり判定コンポーネントクラス
/// </summary>
class Collider : public Component
{
public:

	// 当たり判定判別用タグ
	enum TypeID
	{
		Player,
		PlayerBullet,
		PlayerSword,
		Enemy,
		EnemyBullet,
		Wall,
		DropItem,
		DropItemB,
		Floor
	};

	// 当たり判定の処理の種類
	enum ColliderType
	{
		Fixed,
		Collision,
		Trigger
	};

	// コンストラクタ
	Collider();
	// デストラクタ
	~Collider();

	// 初期化処理
	void Initialize(TypeID id,ColliderType colliderType,
		DirectX::SimpleMath::Vector3 size = DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector3 initialPos = DirectX::SimpleMath::Vector3::Zero);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();

	// バウンディングボックスを取得
	DirectX::BoundingBox* GetBoundingBox() { return m_boudingBox.get(); }
	// タグIDを取得
	TypeID GetTypeID() { return m_typeID; }
	// 当たり判定の種類を取得
	ColliderType GetColliderType() { return m_colliderType; }
	// 当たり判定をオン/オフ
	void SetActive(bool isActive) { m_isActive = isActive; }
	// Activeな当たり判定か取得
	bool GetActive() { return m_isActive; }

	// 押し戻し処理
	static void CheckHit(GameObject* object1, GameObject* object2);

private:
	Graphics* m_graphics;
	TypeID m_typeID;
	ColliderType m_colliderType;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
	DirectX::SimpleMath::Vector3 m_initalePosition;
	bool m_isActive;
};

