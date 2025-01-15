#pragma once
#include "Game/UI/UI.h"

class Tutorial
{
public:
	Tutorial();
	~Tutorial();

	void Update(float elapsedTime);
	void Render();

private:
	std::unique_ptr<UI> m_ui;
	float m_totalTime;
};
