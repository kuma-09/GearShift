#pragma once
#include "Framework/Graphics.h"

class SceneMask
{
public:

	void Update(float elapsedTime);

	const bool IsOpen() { return m_isOpen; }
	const bool IsClose() { return m_isClose; }

	void Open() { m_isOpen = true;   m_alpha = 1.f; }
	void Close() { m_isClose = true; m_alpha = 0.f; }


private:
	bool m_isOpen;
	bool m_isClose;

	float m_alpha;
};