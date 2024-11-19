#pragma once
#include <Framework/Graphics.h>
#include <Framework/RenderTexture.h>

class PostProcess
{
public:
	PostProcess();
	~PostProcess();

	void Initialize();
	void Update(float elapsedTime);
	void BeginNormal();
	void BeginBloom();
	void combinationRT();
	void SetNoise(bool isNoise) { m_isNoise = isNoise; }

private:

	Graphics* m_graphics;

	//	���̓��C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �萔�o�b�t�@
	struct ConstantBuffer
	{
		float time;
		float pad[3];
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// ���_���
	DirectX::VertexPositionTexture m_vertex[4];

	// �|�X�g�v���Z�X�p
	// �����_�[�e�N�X�`��(�V�[���S��)
	std::unique_ptr<DX::RenderTexture> m_offscreenRT;
	std::unique_ptr<DX::RenderTexture> m_offscreenRT_Bloom;
	std::unique_ptr<DX::RenderTexture> m_offscreenRT_Normal;
	std::unique_ptr<DX::RenderTexture> m_finalRenderTexture;

	// �����_�[�e�N�X�`��(�u���[���p)
	std::unique_ptr<DX::RenderTexture> m_blur1RT;
	std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// �x�[�V�b�N�|�X�g�v���Z�X
	std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// �f���A���|�X�g�v���Z�X
	std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

	// �ʏ펞�̃V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
	// �~�b�V�����J�n���̃m�C�Y�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_startVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_startPS;
	// ��e���̃m�C�Y�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_noiseVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_noisePS;
	
	bool m_isStartNoise;
	bool m_isNoise;
	bool m_isBoost;
	float m_nowTime;
	const float m_maxStartNoiseTime = 1.0f;
	const float m_maxNoiseTime = 0.2f;
	const float m_maxBoostBlur = 0.5f;

};
