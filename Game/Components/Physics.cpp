#include "pch.h"
#include "Physics.h"
#include "Collider.h"
#include "Game/GameObject.h"

// コンストラクタ
Physics::Physics()
{
}

// デストラクタ
Physics::~Physics()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Physics::Initialize()
{
	m_gravity = 0;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="elapsedTime"></param>
void Physics::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

	m_gravity += elapsedTime;
	if (m_gravity > MAX_GRAVITY)
	{
		m_gravity = MAX_GRAVITY;
	}
	// GameObjectに重力を適用
	GetOwner()->SetVelocity(GetOwner()->GetVelocity() - Vector3(0, m_gravity, 0));

}
