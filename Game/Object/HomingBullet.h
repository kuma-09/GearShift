#pragma once
#include "Game/GameObject.h"

class HomingBullet :public GameObject
{
public:

	enum BulletState
	{
		UNUSED,
		FLYING,
		USED,
	};

	HomingBullet(IScene* scene, BoxCollider::TypeID id);
	~HomingBullet();
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
	GameObject* m_target;

	const float SPEED = 2.0f;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_velocity;
	float period;
};