#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Menu/UserInterface.h"
#include "Game/Object/SkyDome.h"
#include "Game/Title/TitlePlayer.h"
#include "Game/Title/TitleCamera.h"
#include "Game/Object/Floor.h"

class TitleScene final: public IScene
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

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	std::unique_ptr<tito::UserInterface> m_titleLogo;
	std::unique_ptr<tito::UserInterface> m_backGround;
	std::unique_ptr<TitleCamera> m_camera;
	std::unique_ptr<SkyDome> m_skydome;
	std::unique_ptr<TitlePlayer> m_player;
	DirectX::Model* m_floorModel;
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;

	
};

