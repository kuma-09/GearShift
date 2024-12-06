#pragma once
#include "Game/State.h"

class Enemy;

class BossTackleState : public State
{
public:
	BossTackleState(Enemy* enemy);

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();
private:
	Enemy* m_enemy;

	const float STATE_INTERVAL = 1.0f;
	const float SPEED = 0.3f;
	float m_totalTime;
};
