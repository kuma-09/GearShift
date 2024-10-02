#pragma once
#include "Game/GameObject.h"
#include "Wall.h"

class BillA :public GameObject
{
public:
	BillA(IScene* scene);
	~BillA();

	void Initialize();
	void Update(float elapsedTime);
	void Render();

private:

};
