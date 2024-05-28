#pragma once

class GameObject;

class IComponent
{
public:
	IComponent() {};
	virtual ~IComponent() = default;

	virtual void Update(float elapsedTime) = 0;

	// �e���w��
	void SetOwner(GameObject* a_pObject)
	{
		m_pOwner = a_pObject;
	}

	GameObject* GetOwner() { return m_pOwner; }

private:

	// �e
	GameObject* m_pOwner = nullptr;
};
