#pragma once
#include "Game/GameObject.h"

class Cloud1 : public GameObject
{
public:
	Cloud1();
	~Cloud1();

	void Initialize();
	void Update(float elapsedTime);

private:

};