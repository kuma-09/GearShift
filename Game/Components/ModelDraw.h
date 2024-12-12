#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"


class ModelDraw : public IComponent
{
public:
	ModelDraw();
	~ModelDraw();

	void Initialize(DirectX::Model* model,bool texture = false);
	void Update(float elapsedTime);
	void Render();
	void CreateShadow();
	void Finalize();

private:
	Graphics* m_graphics;
	DirectX::Model* m_model;
	DirectX::SimpleMath::Matrix m_world;
	bool m_isTexture;
};

