/*
	@file	HitParticle.cpp
	@brief	��ʓI�ȃV�[���N���X
*/
#include "pch.h"
#include "HitParticle.h"
#include <cassert>
#include "Framework/Resources.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


//---------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------
HitParticle::HitParticle()
	:
	m_basicEffect{},
	m_primitiveBatch{},
	m_inputLayout{},
	m_projection{},
	ALPHA{},
	RG{},
	m_vertices{}
{
	
}

//---------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------
HitParticle::~HitParticle()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void HitParticle::Initialize(Vector3 pos, Vector3 vel)
{

	

	auto device =  Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();



	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);	// ���C�e�B���O(OFF)
	m_basicEffect->SetVertexColorEnabled(false);// ���_�J���[(OFF)
	m_basicEffect->SetTextureEnabled(true);		// �e�N�X�`��(ON)

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)
	);

	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	

	m_vertices[0] = {Vector3( 0.0f, 0.0f, 0.0f) ,  Vector2(0.0f,0.0f)};
	m_vertices[1] = {Vector3( 0.25f, 0.0f, 0.0f) , Vector2(1.0f,0.0f)};
	m_vertices[2] = {Vector3( 0.0f, 0.25f, 0.0f) , Vector2(0.0f,1.0f)};
	m_vertices[3] = {Vector3( 0.25f, 0.25f, 0.0f) ,Vector2(1.0f,1.0f)};
	m_pos = pos;
	m_vel = vel;
	// �F�ƃA���t�@�l��������
	ALPHA = 1.0f;
	RG = 0.0f;

	// �摜�����[�h����
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,									// �f�o�C�X�R���e�L�X�g
			L"Resources/Textures/particle.png",	// �摜�t�@�C���̃p�X
			nullptr,								// �����I�ȃe�N�X�`��
			m_texture.ReleaseAndGetAddressOf()		// �V�F�[�_���\�[�X�r���[(�\���p)
		)
	);

}

//---------------------------------------------------------
// �X�V����
//---------------------------------------------------------
bool HitParticle::Update()
{
	m_pos += m_vel;
	ALPHA -= 0.02f;
	if (ALPHA < 0.3f)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------
// �`�悷��
//---------------------------------------------------------
void HitParticle::Render(Matrix view, Matrix projection)
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();

	// �e��p�����[�^���X�V����
	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);// �u�����h�X�e�[�g
	ID3D11SamplerState* sampler = states->PointWrap();	// or PointWrap()
	context->PSSetSamplers(0, 1, &sampler);
	context->OMSetDepthStencilState(states->DepthRead(), 0);		// �[�x�o�b�t�@/�X�e���V���o�b�t�@
	context->RSSetState(states->CullClockwise());						// �J�����O
	context->IASetInputLayout(m_inputLayout.Get());					// ���̓��C�A�E�g


	// ���[���h�s����X�V����
	Matrix billboardMat = view.Invert();
	billboardMat._41 = 0;
	billboardMat._42 = 0;
	billboardMat._43 = 0;

	// �e��ϊ�
	Matrix world = Matrix::CreateScale(1.0f);
	world *= Matrix::CreateTranslation(m_pos);
	billboardMat *= world;


	// �G�t�F�N�g��ύX���遨���W�n��ݒ肷��
	m_basicEffect->SetWorld(billboardMat);				// ���[���h�s��
	m_basicEffect->SetView(view);				// �r���[�s��
	m_basicEffect->SetProjection(projection);	// �ˉe�s��
	m_basicEffect->SetTexture(m_texture.Get());	// �e�N�X�`��
	m_basicEffect->SetColorAndAlpha(Vector4(0.5f, 0.7f, 1.f, ALPHA));



	m_basicEffect->Apply(context);				// �x�[�V�b�N�G�t�F�N�g���X�V����


	// �v���~�e�B�u�o�b�`�ŕ`�悷��
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_primitiveBatch->End();

}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void HitParticle::Finalize()
{
	// do nothing.
}
