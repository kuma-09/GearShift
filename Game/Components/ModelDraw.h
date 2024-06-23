#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class ModelDraw : public IComponent
{
public:
	ModelDraw();
	~ModelDraw();

	enum ModelType
	{
		Player,
		Head,
		BodyTop,
		BodyBottom,
		LArm,
		RArm,
		LLeg,
		RLeg,
		Dice
	};

	void Initialize();
	void Update(float elapsedTime);
	void Render(ModelType type,DirectX::SimpleMath::Matrix world,bool black);
	void Finalize();

private:
	void SetBlack();
private:
	Graphics* m_graphics;
	Resources* m_resources;
};

