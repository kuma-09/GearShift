#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class Head : public Part
{
public:
	Head();
	~Head();

	void Initialize(int hp,IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;
};

