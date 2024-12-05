#pragma once
#include "Game/State.h"

class Enemy;

class BossGatlingState : public State
{
public:
	BossGatlingState(Enemy* enemy);

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();
private:
	Enemy* m_enemy;

	const float SPEED = 0.1f;
	const float STATE_INTERVAL = 1.0f;
	const float SHOT_INTERVAL = 0.1f;
	float m_bulletInterval;
	float m_totalTime;
};
