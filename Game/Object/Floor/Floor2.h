#pragma once
#include "Game/GameObject.h"

class Floor2 : public GameObject
{
public:
	Floor2(IScene* scene);
	void Initialize();
	void Update(float elapsedTime);
	void Render();
};
