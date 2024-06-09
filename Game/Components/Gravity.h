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

private:
	float m_velocity;
};

