#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"


class Enemy
{
public:

	void Initialize();

	void Update();

	void Render();

	void Finalize();

	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;


	std::unique_ptr<DirectX::Model> m_model;

	std::unique_ptr<DirectX::BoundingBox> m_boundingBox;

	const float SPEED_FB = 0.1f;
	const float SPEED_RL = 0.05f;

	float m_angle;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_center;
};

