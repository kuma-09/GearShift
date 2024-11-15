#include "pch.h"
#include "Reload.h"

Reload::Reload()
{
}

Reload::~Reload()
{
}

void Reload::Initialize()
{
	m_nowTime = 0;
	m_ui = std::make_unique<UI>(L"Resources/Textures/bar.png");
}

void Reload::Update(float elapsedTime)
{
	m_nowTime += elapsedTime * 5;
	float n = sinf(m_nowTime) / 2 + 0.5f;
	m_color = { n,n,n,n };

}

void Reload::Render()
{
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	m_ui->Render({ float(windowSize.right) / 2,float(windowSize.bottom) / 2 }, m_color);
}
