#pragma once
#include "Components/IComponent.h"
#include "Framework/Graphics.h"
#include "Player/Player.h"

class Camera : public IComponent
{
public:
	Camera();
	~Camera();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetTarget(GameObject* player, GameObject* enemy);

private:
	Graphics* m_graphics;

	GameObject* m_player;
	GameObject* m_enemy;
};

