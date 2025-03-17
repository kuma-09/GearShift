#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class RightArm : public Part
{
public:
	RightArm();
	~RightArm();

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
};

