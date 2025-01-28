#pragma once
#include "Framework/Graphics.h"

class UI
{
public:
	UI(const wchar_t* path);
	~UI();

	void Initialize();
	void Render(DirectX::SimpleMath::Vector2 pos, DirectX::XMVECTORF32 color = DirectX::Colors::White,
		DirectX::SimpleMath::Vector2 origin = DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2 size = DirectX::SimpleMath::Vector2::One);

	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }
	void SetTexture(const wchar_t* path);


private:

	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	Graphics* m_graphics;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	DirectX::SimpleMath::Vector2 m_position;

};