#include "pch.h"
#include "ReloadUI.h"

ReloadUI::ReloadUI()
{
}

ReloadUI::~ReloadUI()
{
}

void ReloadUI::Initialize()
{
	m_nowTime = 0;
	m_ui = std::make_unique<UI>(L"Resources/Textures/Reload.png");
}

void ReloadUI::Update(float elapsedTime)
{
	m_nowTime += elapsedTime * 5;
	float n = sinf(m_nowTime) / 2 + 0.5f;
	m_color = { n,n,n,n };
}

void ReloadUI::Render()
{
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_ui->Render({ float(windowSize.right) / 2,float(windowSize.bottom) / 2  + 150}, m_color,DirectX::SimpleMath::Vector2(643,121) / 2, { 0.25f, 0.25f });
}
