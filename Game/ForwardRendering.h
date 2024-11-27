#pragma once
#include "Framework/RenderTexture.h"

class ForwardRendering
{
public:
	static void Initialize();
	static void BeginBuffer();

	static DX::RenderTexture* GetRenderTexture() { return m_renderTexture.get(); }
private:
	// �����_�[�e�N�X�`��
	static std::unique_ptr<DX::RenderTexture> m_renderTexture;
	//// GBuffer�������ݗp�̃V�F�[�_�[
	//static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader_tex;
	//// DeferredLighting���̃V�F�[�_�[
	//static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader_light;
	//static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader_light;
	////�萔�o�b�t�@
	//struct ConstBuffer
	//{
	//	DirectX::XMMATRIX matView;
	//	DirectX::XMMATRIX matProj;
	//	DirectX::XMMATRIX inverseViewProj;
	//};
	//static Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	//// �C���v�b�g���C�A�E�g
	//static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayoutGBuffer;
	//static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayoutLight;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT_G;
	//static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT_L;
	//// �v���~�e�B�u�o�b�`
	//static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//// ���_���
	//static DirectX::VertexPositionTexture m_vertex[4];
};

