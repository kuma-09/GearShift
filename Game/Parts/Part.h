#pragma once
#include "Game/GameObject.h"

class Part :public GameObject
{
public:
	Part() {};
	virtual ~Part() = default;

	virtual void Initialize(int hp, IScene* scene) = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(DirectX::SimpleMath::Matrix world) = 0;

	// êeÇéwíË
	void SetOwner(GameObject* a_pObject)
	{
		m_pOwner = a_pObject;
	}

	GameObject* GetOwner() { return m_pOwner; }

	enum TypeID
	{
		Head,
		BodyTop,
		LeftArm,
		RightArm,
		BodyBottom,
		LeftLeg,
		RightLeg
	};

	void Action();

	void SetTypeID(TypeID id) { m_typeID = id; }
	TypeID GetTypeID() { return m_typeID; }


private:

	// êe
	GameObject* m_pOwner = nullptr;
	TypeID m_typeID;
};
