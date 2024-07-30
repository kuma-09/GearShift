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
		Dice,
		Cube
	};

	void Initialize(ModelType type);
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Matrix world);
	void Render(DirectX::SimpleMath::Matrix world, DirectX::XMVECTORF32 color);
	void Finalize();

private:
	void SetColor(DirectX::XMVECTORF32 color);
private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	DirectX::Model* m_model;
};

