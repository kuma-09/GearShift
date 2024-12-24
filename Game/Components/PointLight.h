#pragma once
#include "IComponent.h"

class PointLight : public IComponent
{
public:
	PointLight();
	~PointLight();

	void Initialize(DirectX::SimpleMath::Vector3 position,DirectX::SimpleMath::Vector3 = {1,1,1});
	void Update(float elapsedTime);

	void ClearColor();
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	DirectX::SimpleMath::Vector3 GetColor() { return m_nowColor; }

private:
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_targetColor;
	DirectX::SimpleMath::Vector3 m_nowColor;
	float m_nowTime;
	float m_range;
};

