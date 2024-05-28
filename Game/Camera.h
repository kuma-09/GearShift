#pragma once
#include "GameObject.h"
#include "Framework/Graphics.h"
#include "Player/Player.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update(float elapsedTime);
	void Update(float elapsedTime, GameObject* player);
	void Render();
	void Finalize();

private:
	Graphics* m_graphics;
	Player* m_player;
};

