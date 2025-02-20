#pragma once
#include "Framework/RenderTexture.h"

class Graphics;
class BaseScene;

class DeferredRendering
{
public:
	static void Initialize();
	static void BeginGBuffer();
	static void DrawGBuffer(bool texture = false,bool rim = false, DirectX::XMVECTORF32 rimColor = DirectX::Colors::White);
	static void DeferredLighting();

	static void GBufferShow();

	static DX::RenderTexture* GetAlbedoRenderTexture() { return s_albedoRT.get(); }
	static DX::RenderTexture* GetNormalRenderTexture() { return s_normalRT.get(); }
	static DX::RenderTexture* GetDepthRenderTexture() { return s_depthRT.get(); }
	static DX::RenderTexture* GetFinalRenderTexture() { return s_finalRT.get(); }
private:
	static Graphics* s_graphics;
	// �����_�[�e�N�X�`��
	static std::unique_ptr<DX::RenderTexture> s_albedoRT;
	static std::unique_ptr<DX::RenderTexture> s_normalRT;
	static std::unique_ptr<DX::RenderTexture> s_depthRT;
	static std::unique_ptr<DX::RenderTexture> s_finalRT;
	// GBuffer�������ݗp�̃V�F�[�_�[
	static Microsoft::WRL::ComPtr<ID3D11VertexShader> s_vertexShader;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> s_pixelShader;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> s_pixelShader_tex;
	// DeferredLighting���̃V�F�[�_�[
	static Microsoft::WRL::ComPtr<ID3D11VertexShader> s_vertexShader_light;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> s_pixelShader_light;
	// Deferred+Forward�p�̃V�F�[�_�[
	static Microsoft::WRL::ComPtr<ID3D11VertexShader> s_vertexShader_combient;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>  s_pixelShader_combient;

	//�萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matProj;
		DirectX::XMMATRIX inverseViewProj;
		DirectX::XMMATRIX lightView[4];
		DirectX::XMMATRIX lightProj[4];
		int               lightNum;
		DirectX::XMVECTOR lightPos[128];
		DirectX::XMVECTOR lightColor[128];
	};

	struct ConstBuffer_RimLight
	{
		bool isRim;
		DirectX::XMVECTOR rimColor;
	};

	static Microsoft::WRL::ComPtr<ID3D11Buffer> s_constantBuffer;
	static Microsoft::WRL::ComPtr<ID3D11Buffer> s_constantBuffer_rim;
	// �T���v���[
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;
	static DirectX::SimpleMath::Matrix  s_lightViewProj;
	static DirectX::SimpleMath::Vector3 s_lightPosition;

	// �C���v�b�g���C�A�E�g
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> s_inputLayoutGBuffer;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayoutLight;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT_G;
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT_L;
	// �X�v���C�g�o�b�`(�f�o�b�O�p
	static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;
	// �v���~�e�B�u�o�b�`
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> s_batch;
	// ���_���
	static DirectX::VertexPositionTexture s_vertex[4];
};

