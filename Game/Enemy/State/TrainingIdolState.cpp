#include "pch.h"
#include "TrainingIdolState.h"

TrainingIdolState::TrainingIdolState()
{
	m_totalTime = 0;
}

void TrainingIdolState::Initialize()
{
}

void TrainingIdolState::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
}

void TrainingIdolState::Render()
{
}

void TrainingIdolState::Finalize()
{
}
