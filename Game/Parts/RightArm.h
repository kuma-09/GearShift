#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"
#include "Game/Player/Gun.h"

class RightArm : public Part
{
public:
	RightArm();
	~RightArm();

	void Initialize(int hp,IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
	std::unique_ptr<Gun> m_gun;
};

