#pragma once
#include "Game/GameObject.h"

class BurnerVertical : public GameObject
{
public:
	BurnerVertical();
    ~BurnerVertical();

	void Initialize();
	void Update(float elapsedTime);

	void Collision(Collider* collider) {};

private:
};
