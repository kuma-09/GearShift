#pragma once
#include "Game/GameObject.h"

class Floor : public GameObject
{
	Floor();

	void Update(float elapsedTime);
	void Render();
};