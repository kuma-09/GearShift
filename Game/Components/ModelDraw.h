#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class Shader;

class ModelDraw : public IComponent
{
public:
	ModelDraw();
	~ModelDraw();

	void Initialize(DirectX::Model* model);
	void Update(float elapsedTime);
	void Render(bool texture = false, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void Render(DirectX::SimpleMath::Matrix worldbool, bool texture = false, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void CreateShadow();
	void OutLineRender();
	void Finalize();

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	DirectX::Model* m_model;
	std::unique_ptr<Shader> m_shader;
};

