#pragma once
#include <Framework/Graphics.h>
#include <Framework/RenderTexture.h>

class Noise
{
public:
	static void Initialize();
	static void Update(float elapsedTime);

	static void SetNoise(bool isNoise);
	static void SetHitNoise(bool isNoise);

	// �ŏI���ʂɃm�C�Y��K�p
	static void ApplyNoise(ID3D11ShaderResourceView* srv);

private:

	static Graphics* m_graphics;

	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �萔�o�b�t�@
	struct ConstantBuffer
	{
		float time;
		float pad[3];
	};

	static Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// ���_���
	static DirectX::VertexPositionTexture m_vertex[4];

	// �v���~�e�B�u�o�b�`
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// �m�C�Y�V�F�[�_�[
	static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_noiseVS;
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_noisePS;

	// ��e���̃m�C�Y�V�F�[�_�[
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_hitNoisePS;

	static bool  m_isNoise;
	static bool  s_isHitNoise;
	static float m_nowTime;
	static const float PIXELSIZE;
	static const float m_maxNoiseTime;
};
