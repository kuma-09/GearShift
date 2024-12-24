#include "pch.h"
#include "PointLight.h"
#include "Game/Manager/PointLightManager.h"
#include "Game/GameObject.h"
#include "Framework/Easing.h"

PointLight::PointLight()
{
	PointLightManager::Add(this);
	m_nowColor = { 0,0,0 };
	m_targetColor = { 0,0,0 };
	m_nowTime = -1.0f;
}

PointLight::~PointLight()
{
	PointLightManager::Remove(this);
}


void PointLight::Initialize(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 color)
{
	m_position = position;
	m_targetColor = color;
	m_nowTime = 0.0f;
}

void PointLight::Update(float elapsedTime)
{
	m_position = GetOwner()->GetPosition();
	if (m_nowTime >= 0.0f)
	{
		m_nowTime += elapsedTime;
		m_nowColor = Easing::InOutCubic(m_nowColor, m_targetColor, m_nowTime);
	}
}

void PointLight::ClearColor()
{
	m_targetColor = { 0,0,0 };
	m_nowTime = 0.0f;
}
