#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"


class BoxCollider : public IComponent
{
public:
	BoxCollider();
	~BoxCollider();

	void Initialize();
	void Update(float elapsedTime);
	void Render();
	void Finalize();

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
};

