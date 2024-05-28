#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Game/GameObject.h"

class Enemy : public GameObject
{
public:

	Enemy();
	~Enemy();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:

};

