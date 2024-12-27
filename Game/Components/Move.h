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

	DirectX::SimpleMath::Vector3 GetVelocity();
	bool GetIsMove() { return m_isMove; }

private:
	InputManager* m_inputManager;
	DirectX::SimpleMath::Vector3 m_velocity;
	bool m_isMove;

	const float MAX_SPEED = 0.95f;
};

