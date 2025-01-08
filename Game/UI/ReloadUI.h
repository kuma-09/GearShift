#pragma once
#include "UI.h"


class ReloadUI
{
public:
	ReloadUI();
	~ReloadUI();

	void Initialize();
	void Update(float elapsedTime);
	void Render(float reloadTime);
	void CautionRender();

private:
	Graphics* m_graphics;

	float m_nowTime;
	std::unique_ptr<UI> m_ui;
	DirectX::XMVECTORF32 m_color;

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;

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
};