#pragma once

class BoostGage
{
public:
	BoostGage();
	~BoostGage();

	void Render();

	float GetBoostPoint() { return m_boostPoint; }
	void UseBoostPoint(float usePoint) { m_boostPoint -= usePoint; }

private:

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	float m_boostPoint;

	DirectX::VertexPositionColorTexture m_vertex[4];
	DirectX::VertexPositionColorTexture m_backVertex[4];
};
