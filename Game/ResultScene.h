#pragma once
#include "Scene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "UI/UI.h"
#include "UI/Number.h"

class Camera;

class ResultScene final: public Scene
{
public:
	ResultScene();
	~ResultScene();
	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	void RenderUI() override;
	
	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ
	std::unique_ptr<UI> m_result;
	std::unique_ptr<UI> m_backGround;
	std::unique_ptr<UI>     m_clearTime;
	std::unique_ptr<Number> m_time;
};

