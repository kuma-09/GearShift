#pragma once
#include "IComponent.h"

class BoxCollider;

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
	const float MAX_GRAVITY = 0.5f;
	float m_velocity;
};

