#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Game/GameObject.h"

class Bullet;

class Enemy : public GameObject
{
public:

	Enemy(IScene* scene);
	~Enemy();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void Collision(BoxCollider* collider);

private:
	GameObject* m_target;

	const float SHOT_INTERVAL = 3.0f;
	float m_totalTime;
	std::unique_ptr<Bullet> m_bullet;
};

