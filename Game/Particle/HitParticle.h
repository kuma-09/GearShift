/*
	@file	HitParticle.h
	@brief	��ʓI�ȃV�[���N���X
*/
#pragma once
#include "Framework/Graphics.h"

// ��ʓI�ȃV�[���N���X
class HitParticle
{
private:

	// �x�[�V�b�N�G�t�F�N�g
	// �e�N�X�`���}�b�s���O�A���_�J���[�A���C�e�B���O�����T�|�[�g����
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �v���~�e�B�u�o�b�`
	// ����O�p�`�Ȃǂ̃W�I���g����`�悷�邽�߂ɕK�v
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// ���̓��C�A�E�g
	// ���_���̎�ނ��w�肷��
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;


	// ���ȉ��A�ǋL����ϐ��Ȃǁ�

	// �O�p�`�Ŏg�p����ϐ�
	DirectX::VertexPositionTexture    m_vertices[4];

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;	// �u�����h�X�e�[�g

	float							ALPHA;
	float							RG;
	
	DirectX::SimpleMath::Vector3 m_pos;
	DirectX::SimpleMath::Vector3 m_vel;


public:
	HitParticle();
	~HitParticle();

	void Initialize(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, DirectX::SimpleMath::Vector2 size = {0.25f,0.25f});
	bool Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Finalize();

	float GetAlpha() { return ALPHA; }

	static int get_rand(int min_val, int max_val) {
		return (int)((rand() % (max_val + 1 - min_val)) + min_val);
	}
};
