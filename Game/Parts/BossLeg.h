#pragma once

#include "Part.h"

class BossLeg : public Part
{
public:
	BossLeg();
	~BossLeg();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

	void Collision(Collider* collider);

	void Action();

private:
	bool m_isHit;

};