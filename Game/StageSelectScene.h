#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Object/SkyDome.h"
#include "Game/Title/TitlePlayer.h"
#include "Game/Title/TitleCamera.h"
#include "UI/UI.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"
#include "Game/UI/Menu.h"

class StageSelectScene final : public IScene
{
public:

	void Initialize(Game* game) override;

	void Update(float elapsedTime) override;

	void Render() override;

	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<TitleCamera> m_camera;
	std::unique_ptr<SkyDome> m_skydome;
	std::unique_ptr<TitlePlayer> m_player;
	std::unique_ptr<UI> m_menuBack;
	std::unique_ptr<Menu> m_menu;

	float m_nowTime;
	float m_menuPosition;

	bool m_isStageSelect;

};

