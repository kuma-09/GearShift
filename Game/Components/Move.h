#pragma once
#include "IComponent.h"
#include "Framework/InputManager.h"


class Move : public IComponent
{
public:
	Move();
	~Move();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	DirectX::SimpleMath::Vector3 GetVelocity();

private:
	InputManager* m_inputManager;
	DirectX::SimpleMath::Vector3 m_velocity;

	const float MAX_SPEED = 0.95f;
};

