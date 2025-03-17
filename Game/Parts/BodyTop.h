#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class BodyTop : public Part
{
public:
	BodyTop();
	~BodyTop();

	void Initialize();
	void Update(float elapsedTime);

	void Collision(Collider* collider);

private:
	bool m_isHit;

};

