#pragma once
#include "IComponent.h"
#include "Framework/InputManager.h"


class InputComponent : public IComponent
{
public:
	InputComponent();
	~InputComponent();

	void Initialize();
	void Update(float elapsedTime);

	DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; }

private:
	InputManager* m_inputManager;
	DirectX::SimpleMath::Vector3 m_velocity;

	const float MAX_SPEED = 0.95f;
};

