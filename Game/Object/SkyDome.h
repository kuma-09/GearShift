#pragma once
#include "Game/GameObject.h"
#include "Framework/Graphics.h"

class SkyDome :public GameObject
{
public:
	SkyDome();
	~SkyDome();

	void Update(float elapsedTime);
	void Render();

private:
	Graphics* m_graphics;
};
