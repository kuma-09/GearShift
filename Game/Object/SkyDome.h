#pragma once
#include "Game/GameObject.h"
#include "Framework/Graphics.h"

class SkyDome :public GameObject
{
public:
	SkyDome();
	~SkyDome();

	void Initialize(DirectX::SimpleMath::Vector3 pos);
	void Update(float elapsedTime);
	void Render();

private:
	Graphics* m_graphics;

};
