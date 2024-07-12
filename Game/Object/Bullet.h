#pragma once
#include "Game/GameObject.h"

class Bullet :public GameObject
{
public:

	enum BulletState
	{
		UNUSED,
		FLYING,
		USED,
	};

	Bullet(IScene* scene, BoxCollider::TypeID id);
	~Bullet();
	void Initalize(GameObject* object);
	void Shot(GameObject* object);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	BulletState GetState() { return m_state; }
	void SetState(BulletState state) { m_state = state; }

	void Collision(BoxCollider* collider);


private:
	BulletState m_state;
	GameObject* m_owner;
};