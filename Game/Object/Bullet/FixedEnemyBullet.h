#pragma once
#include "Bullet.h"

class Player;

class FixedEnemyBullet :public Bullet
{
public:

	FixedEnemyBullet(IScene* scene, Collider::TypeID id);
	~FixedEnemyBullet();
	void Initialize(GameObject* object);
	void Shot(GameObject* target);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	void Collision(Collider* collider);

private:

	const float SPEED = 2.5f;
	const float DIFFUSION = 0.5f;

};