#pragma once
#include "Bullet.h"

class Player;

class NormalBullet :public Bullet
{
public:

	NormalBullet(IScene* scene, BoxCollider::TypeID id);
	~NormalBullet();
	void Initalize(GameObject* object);
	void Shot(GameObject* target);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	void Collision(BoxCollider* collider);


private:
	DirectX::SimpleMath::Vector3 LinePrediction(Player* target);
	float PlusMin(float a, float b);

private:

	const float MAX_TIME = 10.0f;
	float m_totalTime;
	const float SPEED = 100.0f;
	const float DIFFUSION = 0.5f;

};