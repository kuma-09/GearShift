#pragma once
#include "IPart.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class BodyBottom : public IPart
{
public:
	BodyBottom();
	~BodyBottom();

	void Initialize();
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

private:
};

