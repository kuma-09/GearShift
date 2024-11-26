#pragma once

#include "Part.h"

class BossLeg : public Part
{
public:
	BossLeg();
	~BossLeg();

	void Initialize(int hp, IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(BoxCollider* collider);

	void Action();

private:
	bool m_isHit;

};