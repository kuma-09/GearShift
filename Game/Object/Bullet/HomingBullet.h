#pragma once
#include "Bullet.h"

class HomingBullet :public Bullet
{
public:

	HomingBullet(IScene* scene, Collider::TypeID id);
	~HomingBullet();
	void Initialize(GameObject* object);
	void Shot(GameObject* object);
	void Shot(GameObject* object, float period);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	void Collision(Collider* collider);


private:

	const float SPEED = 2.0f;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_velocity;
	float m_period;
};