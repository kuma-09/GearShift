#pragma once
#include "Part.h"

class BossHead : public Part
{
public:
	BossHead();
	~BossHead();

	void Initialize(GameObject* target);
	void Update(float elapsedTime);
	void Render() ;
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
};

