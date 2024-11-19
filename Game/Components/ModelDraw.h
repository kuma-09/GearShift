#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"


class ModelDraw : public IComponent
{
public:
	ModelDraw();
	~ModelDraw();

	void Initialize(DirectX::Model* model);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix world, bool texture = false, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void CreateShadow(DirectX::SimpleMath::Matrix world);
	void Finalize();

private:
	Graphics* m_graphics;
	DirectX::Model* m_model;
};

