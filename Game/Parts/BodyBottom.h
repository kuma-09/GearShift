#pragma once
#include "IPart.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class BodyBottom : public IPart
{
public:
	BodyBottom();
	~BodyBottom();

	void Initialize(int hp, IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

	void Collision(BoxCollider* collider);

private:

	bool m_isHit;
};

