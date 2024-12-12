#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"


class ShadowMap
{
public:
	static void Initialize();
	static void BeginDepth();
	static void RenderDepth();
	static void EndDepth();

	static void ShadowMapShow();

private:
	// �f�o�C�X���\�[�X
	static Graphics* m_graphics;

	static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;

	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �T���v���[
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// ���C�g------------------
	// ���C�g�̈ʒu
	static DirectX::SimpleMath::Vector3 m_lightPosition;

	// ���C�g�̉�]
	static DirectX::SimpleMath::Quaternion m_lightRotate;

	// ���C�g�͈̔͂̊p�x
	static float m_lightTheta;

	// �V�F�[�_�[--------------
	// ���_�V�F�[�_�[(�[�x�p)
	static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;
	// �s�N�Z���V�F�[�_�[(�[�x�p)
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// �萔�o�b�t�@-------------
	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR lightPosition;	// ���C�g�̈ʒu
		DirectX::XMVECTOR color;			// ���f���̐F
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	static Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	// �V���h�E�}�b�v�e�N�X�`��---
	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE = 512;

	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	static std::unique_ptr<DX::RenderTexture> m_shadowMapRT;
	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	static std::unique_ptr<DepthStencil> m_shadowMapDS;
};
