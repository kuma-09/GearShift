#pragma once
#include "Game/GameObject.h"

class State : public GameObject
{
public:

	void Initialize();

	void Update(float elapsedTime);

	void Render();

	void Finalize();
};