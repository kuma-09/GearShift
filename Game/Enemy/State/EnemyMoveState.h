#pragma once
#include "Game/State.h"

class Enemy;

class EnemyMoveState : public State
{
public:

	EnemyMoveState(Enemy* enemy);

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	Enemy* m_enemy;

	const float SPEED = 0.1f;
	const float MOVE_INTERVAL = 3.0f;
	float m_totalTime;
};