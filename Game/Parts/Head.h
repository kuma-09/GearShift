#pragma once
#include "IPart.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class Head : public IPart
{
public:
	Head();
	~Head();

	void Initialize();
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

private:
	Graphics* m_graphics;
	Resources* m_resources;
};

