#pragma once
#include "IComponent.h"

class Collider;

class Physics : public IComponent
{
public:
	Physics();
	~Physics();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	void Reset() { m_velocity = 0; }

private:
	const float MAX_GRAVITY = 0.5f;
	float m_velocity;
	bool m_onFloor;
};

