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

	void Initialize(ModelType type);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix world,bool black);
	void Finalize();

private:
	void SetBlack();
private:
	Graphics* m_graphics;
	Resources* m_resources;

	DirectX::Model* m_model;
};

