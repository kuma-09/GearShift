#pragma once
#include "Game/Object/Wall/Wall.h"

class BillB :public Wall
{
public:
	BillB(IScene* scene);
	~BillB();

	void Initialize() ;
	void Update(float elapsedtime);
	void Render();
};
