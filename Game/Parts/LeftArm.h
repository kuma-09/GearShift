#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class LeftArm : public Part
{
public:
	LeftArm();
	~LeftArm();

	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
};

