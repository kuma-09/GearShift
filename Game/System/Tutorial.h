#pragma once
#include "Game/UI/UI.h"

class Tutorial
{
public:
	Tutorial();
	~Tutorial();
	void Initialize(bool isActive);
	void Update(float elapsedTime);
	void Render();

private:
	std::unique_ptr<UI> m_ui;
	int   m_tutorialNum;
	const int   MAX_TUTORIAL_NUM = 3;
	float m_totalTime;
	const float TARGETTIME = 1.0f;
	const float RETURNTIME = 4.0f;
	const float RESIDUALTIME = 3.0f;

	float m_posX;
	float m_reversePosX;

	bool m_isActive;
};
