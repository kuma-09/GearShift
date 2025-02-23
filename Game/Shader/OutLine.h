#pragma once
#include "Framework/Graphics.h"
#include "Framework/RenderTexture.h"

class OutLine
{
public:
	OutLine();
	~OutLine();

	void Initialize();
	void Update(float elapsedTime);
	void Render(ID3D11ShaderResourceView* depthTexture,ID3D11ShaderResourceView* normalTexture);

	void LoadShader(ID3D11Device* device);

private:

	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// ���_���
	DirectX::VertexPositionTexture m_vertex[4];

	// �����_�[�e�N�X�`��
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	//// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

};
