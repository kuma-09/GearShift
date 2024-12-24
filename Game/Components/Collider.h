#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Game/Manager/CollisionManager.h"


class Collider : public IComponent
{
public:
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

	Collider();
	~Collider();

	void Initialize(TypeID id,
		DirectX::SimpleMath::Vector3 size = DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector3 initialPos = DirectX::SimpleMath::Vector3::Zero);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetInitalePosition(DirectX::SimpleMath::Vector3 pos);
	DirectX::BoundingBox* GetBoundingBox() { return m_boudingBox.get(); }
	TypeID GetTypeID() { return m_typeID; }
	void SetActive(bool isActive) { m_isActive = isActive; }
	bool GetActive() { return m_isActive; }

	static void CheckHit(GameObject* object1, GameObject* object2);

private:
	Graphics* m_graphics;
	TypeID m_typeID;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
	DirectX::SimpleMath::Vector3 m_initalePosition;
	bool m_isActive;
};

