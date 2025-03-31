#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class RightArm : public Part
{
public:
	RightArm();
	~RightArm();

	void Initialize();
	void Update(float elapsedTime);
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
};

