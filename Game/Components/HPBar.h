#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"


class HPBar : public IComponent
{
public:
	HPBar();
	~HPBar();

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Vector3 position);
	void Finalize();

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	int  m_maxHp;
	DirectX::VertexPositionColorTexture m_vertex[4];

};

