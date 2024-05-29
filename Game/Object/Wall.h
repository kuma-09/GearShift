#pragma once
#include "Game/GameObject.h"

class Wall :public GameObject
{
public:
	Wall();
	~Wall();

	void Update(float elapsedTime);
	void Render();

private:

};
