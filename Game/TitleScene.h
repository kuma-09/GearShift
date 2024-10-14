#pragma once
#include "IScene.h"
#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Game/Object/SkyDome.h"
#include "Game/Title/TitlePlayer.h"
#include "Game/Title/TitleCamera.h"
#include "Game/Object/Floor.h"
#include "UI/UI.h"
#include "Framework/RenderTexture.h"
#include "Framework/DepthStencil.h"

class TitleScene final: public IScene
{
public:

	void Initialize(Game* game) override;
	
	void Update(float elapsedTime) override;
	
	void Render() override;
	
	void Finalize() override;
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::unique_ptr<TitleCamera> m_camera;
	std::unique_ptr<SkyDome> m_skydome;
	std::unique_ptr<TitlePlayer> m_player;
	DirectX::Model* m_floorModel;
	std::vector<DirectX::SimpleMath::Vector3> m_floorPosition;
	std::unique_ptr<UI> m_titleLogo;
	
private:

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR lightPosition;	// ���C�g�̈ʒu
		DirectX::XMVECTOR lightDirection;	// ���C�g�̕���
		DirectX::XMVECTOR lightAmbient;		// ���C�g�̊���
	};

	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer2
	{
		float fCosTheta;		// �X�|�b�g���C�g��fov/2
		float pad[3];
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer2;

	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;


	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE = 1024;

	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	std::unique_ptr<DepthStencil> m_shadowMapDS;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Tex;

	// ���C�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ���C�g�̉�]
	DirectX::SimpleMath::Quaternion m_lightRotate;

	// �T���v���[
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// �X�|�b�g���C�g�͈̔͂̊p�x
	float m_lightTheta;

};

