#pragma once
#include "Bullet.h"

class Player;

class LaserBullet :public Bullet
{
public:

	LaserBullet(IScene* scene, Collider::TypeID id);
	~LaserBullet();
	void Initialize(GameObject* object);
	void Shot(GameObject* target);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	void Collision(Collider* collider);

private:
	DirectX::SimpleMath::Vector3 m_startPosition;
	const float SPEED = 2.5f;
	const float DIFFUSION = 0.5f;

};