#pragma once
#include "Game/GameObject.h"

class DropItem :public GameObject
{
public:
	DropItem(IScene* scene);
	~DropItem();
	
	void Update(float elapsedTime);
	void Render();

private:

};
