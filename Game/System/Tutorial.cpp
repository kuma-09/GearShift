#include "pch.h"
#include "Tutorial.h"
#include "Framework/Easing.h"

Tutorial::Tutorial()
{
//	for (int i = 0; i < 4; i++)
//	{
//		m_ui[i] = std::make_unique<UI>(L"Resources/Textures/Tutorial" + std::to_wstring(i+1) + L".png");
//		m_ui[i]->Initialize();
//	}
	m_totalTime = 0;
	m_tutorialNum = 1;
	m_posX = 0;
	m_reversePosX = 0;
	std::wstring fileName = L"Resources/Textures/tutorial0" + std::to_wstring(m_tutorialNum) + L".png";
	m_ui = std::make_unique<UI>(fileName.c_str());
}

Tutorial::~Tutorial()
{
}

void Tutorial::Initialize(bool isActive)
{
	m_isActive = isActive;
	m_totalTime = 0;
	m_tutorialNum = 1;
	m_posX = 0;
	m_reversePosX = 0;
}

void Tutorial::Update(float elapsedTime)
{
	if (!m_isActive)return;
	m_totalTime += elapsedTime;
	m_posX = Easing::InOutQuad(m_totalTime, TARGETTIME);
	if (m_totalTime >= TARGETTIME + RESIDUALTIME)
	{
		m_reversePosX = Easing::InOutQuad(m_totalTime - (TARGETTIME + RESIDUALTIME), TARGETTIME);
		if (m_totalTime >= RETURNTIME + TARGETTIME)
		{
			if (m_tutorialNum >= MAX_TUTORIAL_NUM)
			{
				m_isActive = false;
				return;
			}
			m_tutorialNum++;
			m_totalTime = 0;
			std::wstring fileName = L"Resources/Textures/tutorial0" + std::to_wstring(m_tutorialNum) + L".png";
			m_ui->SetTexture(fileName.c_str());

			m_posX = 0;
			m_reversePosX = 0;
		}
	}
}

void Tutorial::Render()
{
	if (!m_isActive)return;
	DirectX::XMVECTORF32 color = { 1,1,1,1 };
	m_ui->Render({ 1280 - (m_posX * 400 - m_reversePosX * 400),100 },color);
}
