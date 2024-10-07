#include "pch.h"
#include "SceneMask.h"

void SceneMask::Update(float elapsedTime)
{
	if (m_isOpen)
	{
		// �t�F�[�h�C��

		m_alpha -= elapsedTime;

		if (m_alpha <= 0)
		{
			m_isOpen = false;
		}
	}
	else if (m_isClose)
	{
		// �t�F�[�h�A�E�g

		m_alpha += elapsedTime;

		if (m_alpha >= 1)
		{
			m_isClose = false;
			Open();
		}
	}
}

