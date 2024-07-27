#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class BodyBottom : public Part
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

