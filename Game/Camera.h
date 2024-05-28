#pragma once
#include "Components/IComponent.h"
#include "Game/GameObject.h"
#include "Framework/Graphics.h"
#include "Player/Player.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update(float elapsedTime);
	void Update(float elapsedTime, GameObject* player, GameObject* enemy);
	void Render();
	void Finalize();

private:
	Graphics* m_graphics;
};

