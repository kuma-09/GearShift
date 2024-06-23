#pragma once
#include "IPart.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class BodyTop : public IPart
{
public:
	BodyTop();
	~BodyTop();

	void Initialize(int hp) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

private:
};

