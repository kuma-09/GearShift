#include "pch.h"
#include "PointLight.h"
#include "Game/Manager/PointLightManager.h"
#include "Game/GameObject.h"
#include "Framework/Easing.h"

// コンストラクタ
PointLight::PointLight()
{
	m_nowColor = { 0,0,0 };
	m_targetColor = { 0,0,0 };
	m_nowTime = -1.0f;
}

// デストラクタ
PointLight::~PointLight()
{
	PointLightManager::Remove(this);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="position">座標</param>
/// <param name="color">カラー</param>
void PointLight::Initialize( DirectX::SimpleMath::Vector3 color)
{
	PointLightManager::Add(this);
	m_nowColor = { 0,0,0 };
	m_targetColor = color;
	m_nowTime = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime"></param>
void PointLight::Update(float elapsedTime)
{
	// 色を徐々に変化
	if (m_nowTime >= 0.0f)
	{
		m_nowTime += elapsedTime;
		m_nowColor = Easing::InOutCubic(m_nowColor, m_targetColor, m_nowTime);
	}
}

// 色をリセット
void PointLight::ClearColor()
{
	m_targetColor = { 0,0,0 };
	m_nowTime = 0.0f;
}
