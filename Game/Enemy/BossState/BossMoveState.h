#pragma once
#include "Game/State.h"

class Enemy;

class BossMoveState : public State
{
public:
	BossMoveState(Enemy* enemy);

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();
private:
	Enemy* m_enemy;

	const float SPEED = 0.2f;
	const float SHOT_INTERVAL = 0.3f;
	float m_totalTime;
};
