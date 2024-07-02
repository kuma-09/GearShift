#pragma once
#include "Game/GameObject.h"

class IPart :public GameObject
{
public:
	IPart() {};
	virtual ~IPart() = default;

	virtual void Initialize(int hp, IScene* scene) = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(DirectX::SimpleMath::Matrix world) = 0;

	// êeÇéwíË
	void SetOwner(GameObject* a_pObject)
	{
		m_pOwner = a_pObject;
	}

	GameObject* GetOwner() { return m_pOwner; }



private:

	// êe
	GameObject* m_pOwner = nullptr;
};
