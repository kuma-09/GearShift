#include "pch.h"
#include "Tutorial.h"

Tutorial::Tutorial()
{
//	for (int i = 0; i < 4; i++)
//	{
//		m_ui[i] = std::make_unique<UI>(L"Resources/Textures/Tutorial" + std::to_wstring(i+1) + L".png");
//		m_ui[i]->Initialize();
//	}

	m_ui = std::make_unique<UI>(L"Resources/Textures/Tutorial0.png");
	m_ui->Initialize();
	m_totalTime = 0;
}

Tutorial::~Tutorial()
{
}

void Tutorial::Update(float elapsedTime)
{
	m_totalTime += elapsedTime;
}

void Tutorial::Render()
{
	DirectX::XMVECTORF32 color = { 1,1,1,m_totalTime / 10 };
	//m_ui->Render({ 0,0 },color);
}
