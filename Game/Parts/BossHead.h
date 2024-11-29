#pragma once
#include "Part.h"

class BossHead : public Part
{
public:
	BossHead(GameObject* target);
	~BossHead();

	void Initialize(int hp, IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(BoxCollider* collider);

private:
	bool m_isHit;
};

