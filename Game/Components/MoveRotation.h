#pragma once
#include "IComponent.h"
#include "Framework/InputManager.h"


class MoveRotation : public IComponent
{
public:
	MoveRotation();
	~MoveRotation();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

private:
	InputManager* m_inputManager;
};

