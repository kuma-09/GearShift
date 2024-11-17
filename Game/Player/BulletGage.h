#pragma once
#include "Framework/Graphics.h"

class BulletGage
{
public:
	BulletGage();
	~BulletGage();

	void Initialize();
	void Update();
	void Render();

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

	int m_maxBoostPoint;
	int m_boostPoint;
	int m_tmpPoint;
};
