#pragma once
#include "Game/GameObject.h"

class Floor : public GameObject
{
public:
	Floor(IScene* scene);
	void Initialize();
	void Update(float elapsedTime);
	void Render();
};