#pragma once
#include "Game/Object/Wall/Wall.h"

class BillD :public Wall
{
public:
	BillD(IScene* scene);
	~BillD();

	void Initialize();
	void Update(float elapsedtime);
	void CreateShadow();
	void Render();
};
