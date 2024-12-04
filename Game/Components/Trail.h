#pragma once
#include "Framework/Graphics.h"
#include "IComponent.h"

class Trail : public IComponent
{
public:

	Trail();
	void Initialize(const wchar_t* path,int bufferSize);
	void Update(float elapsedTime);                               //頂点バッファを更新する
	void Render(DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void SetPos(DirectX::XMFLOAT3 head, DirectX::XMFLOAT3 tail);  //現在フレームの位置を保存する
	void ClearBuffer();
private:

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	//剣の位置を保存する構造体
	struct PosBuffer
	{
		DirectX::XMFLOAT3 head; //剣の先端の位置
		DirectX::XMFLOAT3 tail; //剣の末端の位置
	};

	//頂点バッファ
	struct TrailVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMVECTOR color;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	std::vector<PosBuffer> m_buffer;
	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	int m_maxBufferSize;
	int m_bufferCount;
};