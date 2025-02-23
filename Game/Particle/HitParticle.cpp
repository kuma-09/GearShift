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
	m_graphics{},
	ALPHA{0},
	RG{},
	m_vertices{}
{

	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

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
// �f�X�g���N�^
//---------------------------------------------------------
HitParticle::~HitParticle()
{
	Finalize();
}

//---------------------------------------------------------
// ����������
//---------------------------------------------------------
void HitParticle::Initialize(Vector3 pos, Vector3 vel, Vector2 size)
{
	m_vertices[0] = {Vector3( 0.0f, 0.0f, 0.0f)     , Vector4(1,1,1,1), Vector2(0.0f,0.0f)};
	m_vertices[1] = {Vector3( size.x, 0.0f, 0.0f)   , Vector4(1,1,1,1), Vector2(1.0f,0.0f)};
	m_vertices[2] = {Vector3( 0.0f, size.y, 0.0f)   , Vector4(1,1,1,1), Vector2(0.0f,1.0f)};
	m_vertices[3] = {Vector3( size.x, size.y, 0.0f) , Vector4(1,1,1,1), Vector2(1.0f,1.0f)};
	m_pos = pos;
	m_vel = vel;
	// �F�ƃA���t�@�l��������
	ALPHA = 1.0f;
	RG = 0.0f;

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

	m_graphics->GetBasicEffect()->SetTexture(m_texture.Get());

	// ���[���h�s����X�V����
	Matrix billboardMat = view.Invert();
	billboardMat._41 = 0;
	billboardMat._42 = 0;
	billboardMat._43 = 0;

	// �e��ϊ�
	Matrix world = Matrix::CreateScale(1.0f);
	world *= Matrix::CreateTranslation(m_pos);
	billboardMat *= world;

	// �v���~�e�B�u�o�b�`�ŕ`�悷��
	m_graphics->DrawPrimitiveBegin(view, projection, billboardMat, Vector4(0.5f, 0.7f, 1.f, ALPHA));
	m_graphics->GetPrimitiveBatch()->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
	m_graphics->DrawPrimitiveEnd();

}

//---------------------------------------------------------
// ��n������
//---------------------------------------------------------
void HitParticle::Finalize()
{
	// do nothing.
}
