#pragma once
#include <Framework/Graphics.h>
#include <Framework/RenderTexture.h>

class Bloom
{
public:
	static void Initialize();
	static void Update(float elapsedTime);

	// �u���[���K�p�p�̃����_�[�^�[�Q�b�g�ɕύX
	static void BeginBloom();
	static void EndBloom(ID3D11ShaderResourceView* srv);

	static void BloomTextureShow();
private:

	static Graphics* m_graphics;

	static std::unique_ptr<DirectX::SpriteBatch> s_spriteBatch;

	// ���_���
	static DirectX::VertexPositionTexture m_vertex[4];

	// �|�X�g�v���Z�X�p
	// �����_�[�e�N�X�`��(�V�[���S��)
	static std::unique_ptr<DX::RenderTexture> m_offscreenRT_Bloom;

	// �����_�[�e�N�X�`��(�u���[���p)
	static std::unique_ptr<DX::RenderTexture> m_blur1RT;
	static std::unique_ptr<DX::RenderTexture> m_blur2RT;

	// �v���~�e�B�u�o�b�`
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// �x�[�V�b�N�|�X�g�v���Z�X
	static std::unique_ptr<DirectX::BasicPostProcess> m_basicPostProcess;

	// �f���A���|�X�g�v���Z�X
	static std::unique_ptr<DirectX::DualPostProcess> m_dualPostProcess;

	static const float PIXELSIZE;
};
