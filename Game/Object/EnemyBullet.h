#pragma once
#include "Game/GameObject.h"

class Player;

class EnemyBullet :public GameObject
{
public:

	enum BulletState
	{
		UNUSED,
		FLYING,
		USED,
	};

	EnemyBullet(IScene* scene, BoxCollider::TypeID id);
	~EnemyBullet();
	void Initalize(GameObject* object);
	void Shot(Player* target);
	void Hit();
	void Update(float elapsedTime);
	void Render();

	BulletState GetState() { return m_state; }
	void SetState(BulletState state) { m_state = state; }

	void Collision(BoxCollider* collider);


private:
	DirectX::SimpleMath::Vector3 LinePrediction(Player* target);
	float PlusMin(float a, float b);



private:
	BulletState m_state;

	GameObject* m_owner;

	const float SPEED = 1.0f;
	const float DIFFUSION = 1.0f;

};