#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"


class BoxCollider : public IComponent
{
public:
	enum TypeID
	{
		Player,
		PlayerBullet,
		Enemy,
		EnemyBullet,
		Wall,
		DropItem
	};

	BoxCollider();
	~BoxCollider();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetSize(DirectX::SimpleMath::Vector3 size);

	DirectX::BoundingBox* GetBoundingBox() { return m_boudingBox.get(); }

	TypeID GetTypeID() { return m_typeID; }
	void SetTypeID(TypeID id) { m_typeID = id; }

	static void CheckHit(GameObject* object1, GameObject* object2);

private:
	Graphics* m_graphics;
	TypeID m_typeID;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
};

