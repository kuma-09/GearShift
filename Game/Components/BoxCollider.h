#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"


class BoxCollider : public IComponent
{
public:
	enum TypeID
	{
		Player,
		Enemy,
		Bullet,
		Wall
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

private:
	Graphics* m_graphics;
	TypeID m_typeID;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
};

