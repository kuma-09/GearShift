#pragma once
#include "Game/GameObject.h"

class BurnerHorizon : public GameObject
{
public:
	BurnerHorizon();
	~BurnerHorizon();

	void Initialize();
	void Update(float elapsedTime);

	void Collision(Collider* collider) {};

private:
};