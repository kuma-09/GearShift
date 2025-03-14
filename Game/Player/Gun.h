#pragma once
#include "Game/GameObject.h"
#include "Game/Object/Bullet/Bullet.h"

class Gun : public GameObject
{
public:
	Gun(GameObject* owner);
	~Gun();
	void Initialize();
	void Update(float elapsedTime);
	void Shot(GameObject* target);
	void Reload();

	size_t GetMagazineSize();
	float GetNowReloadTime();
private:
	GameObject* m_owner;

	// �e�̍ő吔
	static const int MAX_BULLET_COUNT = 30;

	//// �e�z��
	std::vector<std::unique_ptr<Bullet>> m_defaultBullet;
	float m_bulletInterval;
	float m_nowReloadTime;
	bool m_isReload;
	const float RELOAD_TIME = 1.0f;
	const float INTERVAL = 0.25f;
};