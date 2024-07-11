#pragma once
#include "IPart.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class RightArm : public IPart
{
public:
	RightArm();
	~RightArm();

	void Initialize(int hp,IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();


private:
};

