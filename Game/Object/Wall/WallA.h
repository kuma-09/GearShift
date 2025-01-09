#pragma once
#include "Game/Object/Wall/Wall.h"

class WallA :public Wall
{
public:
	WallA(IScene* scene);
	~WallA();

	void Initialize();
	void Update(float elapsedtime);
	void CreateShadow();
	void Render();
};
