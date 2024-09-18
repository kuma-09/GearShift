#pragma once
#include "Game/Components/IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
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

	void SetTarget(GameObject* from, GameObject* to);

private:

	GameObject* m_from;
	GameObject* m_to;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

