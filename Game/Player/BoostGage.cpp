#include "pch.h"
#include "BoostGage.h"
#include "Framework/Graphics.h"

BoostGage::BoostGage()
{
	m_boostPoint = 100;
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext());
}

BoostGage::~BoostGage()
{
}

void BoostGage::Render()
{
}