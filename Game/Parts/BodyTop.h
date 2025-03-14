#pragma once
#include "Part.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class BodyTop : public Part
{
public:
	BodyTop();
	~BodyTop();

	void Initialize(int hp,IScene* scene) override;
	void Update(float elapsedTime) override;
	void Render() override;
	void Finalize();

	void Collision(Collider* collider);

private:
	bool m_isHit;

};

