#pragma once
#include "Bullet.h"

class Player;

class EnemyBullet :public Bullet
{
public:

	EnemyBullet(IScene* scene, Collider::TypeID id);
	~EnemyBullet();
	void Initialize(GameObject* object);
	void Shot(GameObject* target);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	void Collision(Collider* collider);


private:
	DirectX::SimpleMath::Vector3 LinePrediction(Player* target);
	float PlusMin(float a, float b);

private:

	const float SPEED = 1.5f;
	const float DIFFUSION = 1.0f;

};