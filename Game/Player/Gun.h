#pragma once
#include "Game/GameObject.h"

class Gun : public GameObject
{
public:
	Gun(GameObject* owner);
	~Gun();
	void Initialize();
	void Update(float elapsedTime);

private:
	GameObject* m_owner;

	//// ’e”z—ñ
	//std::vector<std::unique_ptr<Bullet>> m_defaultBullet;
	//float m_bulletInterval;
	//const float INTERVAL = 0.25f;
};