#pragma once
#include "Game/State.h"

class Enemy;

class FixedAttackState : public State
{
public:
	FixedAttackState(Enemy* enemy);

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();
private:
	Enemy* m_enemy;

	const float SPEED = 0.1f;
	const float SHOT_INTERVAL = 2.0f;
	float m_totalTime;
};
