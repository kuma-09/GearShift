#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Shader/ShadowMap.h"

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

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<Imase::ShadowMap> m_shadowMap;

};
