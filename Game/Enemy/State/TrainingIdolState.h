#pragma once
#include "Game/State.h"

class TrainingIdolState : public State
{
public:
	TrainingIdolState();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();
private:
	const float SPEED = 0.1f;
	const float SHOT_INTERVAL = 2.0f;
	float m_totalTime;
};
