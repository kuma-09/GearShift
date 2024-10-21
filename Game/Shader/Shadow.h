#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"


class Shadow
{
public:
	Shadow();
	~Shadow();

	void Initialize();
	void BeginDepth();
	void RenderDepth();
	void EndDepth();

	void Draw(bool texture,DirectX::XMVECTORF32 color = DirectX::Colors::White);
	void End();

	ID3D11ShaderResourceView* GetSRV() { return m_srv; }

private:

	Graphics* m_graphics;

	// �f�o�b�O�p
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �T���v���[
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

private:

	// ���C�g------------------
	// ���C�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ���C�g�̉�]
	DirectX::SimpleMath::Quaternion m_lightRotate;

	// ���C�g�͈̔͂̊p�x
	float m_lightTheta;

private:
	// �V�F�[�_�[--------------
	// ���_�V�F�[�_�[(�[�x�p)
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// �s�N�Z���V�F�[�_�[(�[�x�p)
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Tex;

private:
	// �萔�o�b�t�@-------------
	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR lightPosition;	// ���C�g�̈ʒu
		DirectX::XMVECTOR lightDirection;	// ���C�g�̕���
		DirectX::XMVECTOR lightAmbient;		// ���C�g�̊���
		DirectX::XMVECTOR color;			// ���f���̐F
	};

	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer2
	{
		float fCosTheta;		// ���C�g��fov/2
		float pad[3];
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer2;

private:

	ID3D11RenderTargetView* m_rtv;
	ID3D11ShaderResourceView* m_srv;
	ID3D11DepthStencilView* m_dsv;

	// �V���h�E�}�b�v�e�N�X�`��---
	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE = 2048;

	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	std::unique_ptr<DepthStencil> m_shadowMapDS;
};
