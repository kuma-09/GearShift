#include "pch.h"
#include "ModelDraw.h"
#include "Game/GameObject.h"
#include "Game/Shader/Shader.h"
#include "Game/DeferredRendering.h"
#include "Game/Manager/RenderManager.h"
#include "Game/Shader/ShadowMap.h"

// �R���X�g���N�^
ModelDraw::ModelDraw()
	:
	m_model{},
	m_isTexture{},
	m_isRimLight{},
	m_rimColor{},
	m_emissiveColor{}
{
	m_graphics = Graphics::GetInstance();
}

// �f�X�g���N�^
ModelDraw::~ModelDraw()
{
	RenderManager::Remove(this);
}

/// <summary>
/// ����������
/// </summary>
/// <param name="model">�`�惂�f��</param>
/// <param name="texture">�e�N�X�`���̗L��</param>
void ModelDraw::Initialize(DirectX::Model* model,bool texture)
{
	m_model = model;
	m_isTexture = texture;

	// �����_�[�}�l�[�W���[�Ƀ��f���`��R���|�[�l���g��ǉ�
	RenderManager::Add(this);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime"></param>
void ModelDraw::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}


/// <summary>
/// �`�揈��
/// </summary>
void ModelDraw::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		// GBuffer�Ƀ��f����`��
		DeferredRendering::DrawGBuffer(m_isTexture,m_isRimLight,m_rimColor,m_emissiveColor);
	});
}

/// <summary>
/// �V���h�E�}�b�v���쐬
/// </summary>
void ModelDraw::CreateShadow()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto state = m_graphics->GetCommonStates();
	auto view = m_graphics->GetViewMatrix();
	auto projection = m_graphics->GetProjectionMatrix();
	auto world = GetOwner()->GetWorld();

	m_model->Draw(context, *state, world, view, projection, false, [&]
	{
		ShadowMap::RenderDepth();
	});
}

/// <summary>
/// �������C�g�J���[��ݒ�
/// </summary>
/// <param name="color"></param>
void ModelDraw::SetRimLithgColor(DirectX::XMVECTORF32 color)
{
	m_isRimLight = true;
	m_rimColor = color;
}

/// <summary>
/// �G�~�b�V�u�J���[��ݒ�
/// </summary>
/// <param name="color"></param>
void ModelDraw::SetEmissiveColor(DirectX::XMVECTORF32 color)
{
	m_emissiveColor = color;
}
