#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class LeftLeg : public Part
{
public:
	LeftLeg();
	~LeftLeg();

	void Initialize(int hp,IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render(DirectX::SimpleMath::Matrix world);
	void Finalize();

	void Collision(BoxCollider* collider);

	void Action();

private:
	bool m_isHit;

};

