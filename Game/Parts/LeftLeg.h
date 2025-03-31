#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class LeftLeg : public Part
{
public:
	LeftLeg();
	~LeftLeg();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	void Collision(Collider* collider);

	void Action();

private:
	bool m_isHit;

};

