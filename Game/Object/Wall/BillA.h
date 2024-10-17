#pragma once
#include "Game/Object/Wall/Wall.h"

class BillA :public Wall
{
public:
	BillA(IScene* scene);
	~BillA();

	void Initialize() ;
	void Update(float elapsedtime);
	void CreateShadow();
	void Render();
};
