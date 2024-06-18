#pragma once
#include "Game/Components/IComponent.h"
#include "Framework/Graphics.h"
#include "Game/GameObject.h"

class Look : public IComponent
{
public:
	Look();
	~Look();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void SetTarget(GameObject* player, GameObject* enemy);

private:

	GameObject* m_player;
	GameObject* m_enemy;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

