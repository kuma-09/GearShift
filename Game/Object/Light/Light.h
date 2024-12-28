#pragma once

#include "Game/GameObject.h"

class Light : public GameObject
{
public:
	Light(IScene* scene);
	~Light();

	void Initialize();
	void Update(float elapsedtime);
	void Render();

private:

};
