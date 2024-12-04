#pragma once
#include "Framework/Graphics.h"
#include "IComponent.h"

class Trail : public IComponent
{
public:

	Trail();
	void Initialize(const wchar_t* path,int bufferSize);
	void Update(float elapsedTime);                               //���_�o�b�t�@���X�V����
	void Render(DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void SetPos(DirectX::XMFLOAT3 head, DirectX::XMFLOAT3 tail);  //���݃t���[���̈ʒu��ۑ�����
	void ClearBuffer();
private:

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	//���̈ʒu��ۑ�����\����
	struct PosBuffer
	{
		DirectX::XMFLOAT3 head; //���̐�[�̈ʒu
		DirectX::XMFLOAT3 tail; //���̖��[�̈ʒu
	};

	//���_�o�b�t�@
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
	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	int m_maxBufferSize;
	int m_bufferCount;
};