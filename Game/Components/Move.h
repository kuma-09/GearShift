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

private:
	InputManager* m_inputManager;
};

