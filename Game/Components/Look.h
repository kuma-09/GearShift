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

	void Initialize(bool h , bool v);
	void Update(float elapsedTime);
	void Render() {};
	void Finalize() {};

	void SetTarget(GameObject* from, GameObject* to);

private:

	GameObject* m_from;
	GameObject* m_to;

	bool m_h;
	bool m_v;

	DirectX::SimpleMath::Vector3 m_eyePosition;
	DirectX::SimpleMath::Vector3 m_targetPosition;
};

