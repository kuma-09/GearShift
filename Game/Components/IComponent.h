#pragma once

class GameObject;

class IComponent
{
public:
	IComponent() {};
	virtual ~IComponent() = default;

	virtual void Update(float elapsedTime) = 0;

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
