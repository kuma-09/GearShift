#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class ModelDraw : public IComponent
{
public:
	ModelDraw();
	~ModelDraw();

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

private:
	Graphics* m_graphics;
	Resources* m_resources;
};

