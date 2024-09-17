#pragma once
#include "IComponent.h"
#include "Framework/InputManager.h"


class Gravity : public IComponent
{
public:
	Gravity();
	~Gravity();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	void Reset() { m_velocity = 0; }

private:
	const float MAX_GRAVITY = 1.0f;
	float m_velocity;
};

