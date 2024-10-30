#pragma once
#include "Framework/Graphics.h"

class BoostGage
{
public:
	BoostGage();
	~BoostGage();

	void Initialize();
	void Update();
	void Render();

	void SetMaxBoostPoint(float point) { m_maxBoostPoint = point; }
	float GetMaxBoostPoint() { return m_maxBoostPoint; }

	void SetBoostPoint(float point) { m_boostPoint = std::min(m_boostPoint + point,m_maxBoostPoint); }
	float GetBoostPoint() { return m_boostPoint; }
	void UseBoostPoint(float usePoint) { m_boostPoint = std::max(0.0f,m_boostPoint - usePoint); }

private:

	Graphics* m_graphics;

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	DirectX::VertexPositionColorTexture m_vertex[4];

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	struct ConstBuffer
	{
		float rotate;
		float pad[3];
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	float m_maxBoostPoint;
	float m_boostPoint;
	float m_tmpPoint;
};
