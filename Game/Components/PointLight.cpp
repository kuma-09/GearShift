#include "pch.h"
#include "PointLight.h"
#include "Game/Manager/PointLightManager.h"
#include "Game/GameObject.h"
#include "Framework/Easing.h"

// �R���X�g���N�^
PointLight::PointLight()
{
	m_nowColor = { 0,0,0 };
	m_targetColor = { 0,0,0 };
	m_nowTime = -1.0f;
}

// �f�X�g���N�^
PointLight::~PointLight()
{
	PointLightManager::Remove(this);
}

/// <summary>
/// ����������
/// </summary>
/// <param name="position">���W</param>
/// <param name="color">�J���[</param>
void PointLight::Initialize( DirectX::SimpleMath::Vector3 color)
{
	PointLightManager::Add(this);
	m_nowColor = { 0,0,0 };
	m_targetColor = color;
	m_nowTime = 0.0f;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="elapsedTime"></param>
void PointLight::Update(float elapsedTime)
{
	// �F�����X�ɕω�
	if (m_nowTime >= 0.0f)
	{
		m_nowTime += elapsedTime;
		m_nowColor = Easing::InOutCubic(m_nowColor, m_targetColor, m_nowTime);
	}
}

// �F�����Z�b�g
void PointLight::ClearColor()
{
	m_targetColor = { 0,0,0 };
	m_nowTime = 0.0f;
}
