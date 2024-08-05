#pragma once
#include "Game/State.h"

class EnemyAttackState : public State
{
public:
	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();
};
