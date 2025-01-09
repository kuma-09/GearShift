#pragma once
#include "Game/Object/Wall/Wall.h"

class BillC :public Wall
{
public:
	BillC(IScene* scene);
	~BillC();

	void Initialize() ;
	void Update(float elapsedtime);
	void CreateShadow();
	void Render();
};
