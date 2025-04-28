#pragma once
#include "Scene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Object/SkyDome.h"
#include "Game/Title/TitlePlayer.h"
#include "Game/Title/TitleCamera.h"
#include "Game/Object/Floor/Floor.h"
#include "UI/UI.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"
#include "Game/UI/Menu.h"

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene final: public Scene
{
public:
	// 初期化処理
	void Initialize(Game* game) override;
	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render() override;
	// UI描画処理
	void RenderUI();
	// 終了処理
	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<TitleCamera> m_camera;
	std::unique_ptr<SkyDome> m_skydome;
	std::unique_ptr<TitlePlayer> m_player;
	std::unique_ptr<Floor> m_floor;
	std::unique_ptr<UI> m_titleLogo;
	std::unique_ptr<UI> m_menuBack;
	std::unique_ptr<Menu> m_menu;
	std::unique_ptr<Menu> m_stageMenu;
	std::unique_ptr<Menu> m_optionMenu;

	float m_nowTime;
	float m_menuNowTime;
	float m_menuPosition;
	float m_stageMenuPosition;

	bool m_isStageSelect;
	bool m_isOption;
	
};

