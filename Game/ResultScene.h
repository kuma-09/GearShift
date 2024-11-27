#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "UI/UI.h"

class Camera;

class ResultScene final: public IScene
{
public:
	ResultScene();
	~ResultScene();
	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	void RenderUI() {};
	
	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	std::unique_ptr<UI> m_result;
	std::unique_ptr<UI> m_backGround;
	
};

