#pragma once
#include "Framework/Graphics.h"

class SceneMask
{
public:

	SceneMask();

	bool Update(float elapsedTime);
	void Render();

	const bool IsOpen() { return m_isOpen; }
	const bool IsClose() { return m_isClose; }

	void Open() { m_isOpen = true;   m_time = 1.f; }
	void Close() { m_isClose = true; m_time = 0.f; }


private:

	struct ConstBuffer
	{
		float time;
		float pad[3];
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	bool m_isOpen;
	bool m_isClose;
	float m_time;
	float m_windowSize;

	Graphics* m_graphics;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr < DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	DirectX::VertexPositionColorTexture m_vertex[4];
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

};