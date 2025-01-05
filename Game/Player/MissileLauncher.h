#pragma once
#include "Game/GameObject.h"
#include "Game/Object/Bullet/Bullet.h"

class MissileLauncher: public GameObject
{
public:
	MissileLauncher(GameObject* owner);
	~MissileLauncher();
	void Initialize();
	void Update(float elapsedTime);
	void Shot(GameObject* target);
	void Reload();

	size_t GetMagazineSize();
private:
	GameObject* m_owner;

	// íeÇÃç≈ëÂêî
	static const int MAX_BULLET_COUNT = 10;

	//// íeîzóÒ
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	float m_missileInterval;
	const float INTERVAL = 0.1f;
};

