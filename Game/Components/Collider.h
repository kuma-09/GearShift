#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"


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
		Floor
	};

	enum ColliderType
	{
		None,
		Box,
		Sphere,
		Oriented
	};

	Collider();
	~Collider();

	template<typename T>
	void Initialize(std::unique_ptr<DirectX::BoundingBox> collider) {};
	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetSize(DirectX::SimpleMath::Vector3 size);
	void SetInitalePosition(DirectX::SimpleMath::Vector3 pos);

	DirectX::BoundingBox* GetBoundingBox() { return m_boudingBox.get(); }

	TypeID GetTypeID() { return m_typeID; }
	void SetTypeID(TypeID id);

	static void CheckHit(GameObject* object1, GameObject* object2);

private:
	Graphics* m_graphics;
	TypeID m_typeID;
	ColliderType m_colliderType;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
	DirectX::SimpleMath::Vector3 m_initalePosition;
};

