#pragma once
#include "Game/GameObject.h"

class Part :public GameObject
{
public:
	Part()
		:m_typeID{}
	{};
	virtual ~Part() = default;

	virtual void Initialize(int hp, IScene* scene) = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void CreateShadow(); 
	virtual void Render();

	// êeÇéwíË
	void SetOwner(GameObject* a_pObject)
	{
		m_pOwner = a_pObject;
	}

	GameObject* GetOwner() { return m_pOwner; }

	enum TypeID
	{
		None,
		Head,
		BodyTop,
		LeftArm,
		RightArm,
		BodyBottom,
		LeftLeg,
		RightLeg
	};

	void SetTypeID(TypeID id) { m_typeID = id; }
	TypeID GetTypeID() { return m_typeID; }

	void SetMaxHP(float hp) { m_maxHP = hp; }
	float GetMaxHP() { return m_maxHP; }


private:

	// êe
	GameObject* m_pOwner = nullptr;
	TypeID m_typeID;
	float m_maxHP;
};
