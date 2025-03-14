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

	Sword(IScene* scene, Collider::TypeID id);
	~Sword();
	void Initalize(GameObject* object);
	void Shot(GameObject* object);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	bool GetIsHit() { return m_isHit; }
	SwordState GetState() { return m_state; }
	void SetState(SwordState state) { m_state = state; }

	void Collision(Collider* collider);


private:
	SwordState m_state;
	GameObject* m_owner;

	bool m_isHit;
	float m_rotate;

	const DirectX::SimpleMath::Vector3 m_size = {5.f,5.f,5.f};
};