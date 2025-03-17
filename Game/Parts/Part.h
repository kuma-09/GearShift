#pragma once
#include "Game/GameObject.h"

class Part :public GameObject
{
public:
	Part();
	~Part();

	void Initialize() {};
	void Update(float elapsedTime) {};

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

private:
	TypeID m_typeID;
};
