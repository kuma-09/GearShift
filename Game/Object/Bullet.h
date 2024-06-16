#pragma once
#include "Game/GameObject.h"

class Bullet :public GameObject
{
public:
	Bullet();
	~Bullet();
	void Initalize(GameObject* object);
	void Update(float elapsedTime);
	void Render();

private:

};
