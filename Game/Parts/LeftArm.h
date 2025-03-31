#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class LeftArm : public Part
{
public:
	LeftArm();
	~LeftArm();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
};

