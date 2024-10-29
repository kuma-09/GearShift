#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Particle/HitEffect.h"
#include "UI/CircleGage.h"

class TestScene final : public IScene
{
public:
	TestScene();

	void Initialize(Game* game) override;

	void Update(float elapsedTime) override;

	void Render() override;

	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
	std::unique_ptr<HitEffect> m_effect;
	std::unique_ptr<CircleGage> m_gage;

	float m_time;

};
