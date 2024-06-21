#pragma once
#include "Game/GameObject.h"

class Bullet :public GameObject
{
public:
	Bullet(IScene* scene, BoxCollider::TypeID id);
	~Bullet();
	void Initalize(GameObject* object);
	void Update(float elapsedTime);
	void Render();

private:

};
