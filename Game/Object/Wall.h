#pragma once
#include "Game/GameObject.h"

class Wall :public GameObject
{
public:
	Wall(IScene* scene);
	~Wall();

	void Update(float elapsedTime);
	void Render();

private:

};
