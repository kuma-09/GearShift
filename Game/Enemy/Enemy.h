#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"

class Player;

class Enemy
{
public:

	void Initialize();

	void Update();

	void Render();

	void Finalize();

	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	Resources* m_resources;

	Player* m_player;


	std::unique_ptr<DirectX::BoundingBox> m_boundingBox;

	const float SPEED_FB = 0.1f;
	const float SPEED_RL = 0.05f;

	float m_angle;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_center;
};

