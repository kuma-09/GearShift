#pragma once
#include "IPart.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class RightLeg : public IPart
{
public:
	RightLeg();
	~RightLeg();

	void Initialize(int hp) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

private:
};

