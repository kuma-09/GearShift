#pragma once
#include "IComponent.h"
#include "Framework/Graphics.h"
#include "Framework/Resources.h"

class HPBar : public IComponent
{
public:
	HPBar();
	~HPBar();

	void Initialize();
	void Update(float elapsedTime);
	void Render(DirectX::SimpleMath::Vector2 position);
	void Render(DirectX::SimpleMath::Vector3 position);
	void Finalize();

private:
	Graphics* m_graphics;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	int  m_maxHp;
	float m_greenHp;
	float m_redHp;
	float m_nowTime;
	DirectX::VertexPositionColorTexture m_vertex[4];
	DirectX::VertexPositionColorTexture m_redVertex[4];
	DirectX::VertexPositionColorTexture m_backVertex[4];

	ID3D11ShaderResourceView* m_texture;

};

