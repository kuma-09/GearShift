#include "pch.h"	
#include "HitEffect.h"

void HitEffect::Set(DirectX::SimpleMath::Vector3 pos)
{
	m_graphics = Graphics::GetInstance();
	m_positions.push_back(pos);
	m_lifeTimes.push_back(0.0f);
}

void HitEffect::Update(float elapsedTime)
{
	for (auto& lifeTime: m_lifeTimes)
	{
		lifeTime += elapsedTime;
	}
}

void HitEffect::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	
}

void HitEffect::Finalize()
{
}
