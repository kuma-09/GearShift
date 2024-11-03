#pragma once
#include "Bullet.h"

class Player;

class ShotgunBullet :public Bullet
{
public:

	ShotgunBullet(IScene* scene, BoxCollider::TypeID id);
	~ShotgunBullet();
	void Initalize(GameObject* object);
	void Shot(GameObject* target);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	void Collision(BoxCollider* collider);

private:

	const float MAX_TIME = 10.0f;
	float m_totalTime;
	const float SPEED = 100.0f;
	const float DIFFUSION = 0.5f;

};