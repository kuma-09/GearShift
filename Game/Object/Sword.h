#pragma once
#include "Game/GameObject.h"

class Sword :public GameObject
{
public:

	enum SwordState
	{
		UNUSED,
		USING,
		USED,
	};

	Sword(IScene* scene, BoxCollider::TypeID id);
	~Sword();
	void Initalize(GameObject* object);
	void Shot(GameObject* object);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	SwordState GetState() { return m_state; }
	void SetState(SwordState state) { m_state = state; }

	void Collision(BoxCollider* collider);


private:
	SwordState m_state;
	GameObject* m_owner;

	const DirectX::SimpleMath::Vector3 m_size = {5,5,5};
};