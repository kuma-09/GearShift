#include "pch.h"
#include "Physics.h"
#include "Collider.h"
#include "Game/GameObject.h"

// �R���X�g���N�^
Physics::Physics()
{
}

// �f�X�g���N�^
Physics::~Physics()
{
}

/// <summary>
/// ����������
/// </summary>
void Physics::Initialize()
{
	m_gravity = 0;
}

/// <summary>
/// �X�V����
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
	// GameObject�ɏd�͂�K�p
	GetOwner()->SetVelocity(GetOwner()->GetVelocity() - Vector3(0, m_gravity, 0));

}
