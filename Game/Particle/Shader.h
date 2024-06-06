//--------------------------------------------------------------------------------------
// File: Shader.h
//
// �p�[�e�B�N���N���X
//
//-------------------------------------------------------------------------------------

#pragma once
#include "Framework/DeviceResources.h"

class Shader
{
public:
	//	�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};
private:
	//	�ϐ�
	DX::DeviceResources* m_pDR;

	static Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	//	�e�N�X�`���n���h��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
	//	�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

public:
	//	�֐�
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	

	Shader();
	~Shader();

	void CreateShader();

	void RenderStart(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void RenderEnd();

};
