#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"

class PlayScene final: public IScene
{
public:
	void Initialize() override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;
private:

	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	std::unique_ptr<DirectX::Model> m_model;

	float m_angle;

	DirectX::SimpleMath::Vector3 m_position;
};

