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

	DirectX::BoundingBox* GetBoundingBox() { return m_boudingBox.get(); }

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::BoundingBox> m_boudingBox;
};

