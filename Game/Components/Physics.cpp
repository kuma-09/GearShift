#include "pch.h"
#include "Physics.h"
#include "Collider.h"
#include "Game/GameObject.h"


Physics::Physics()
{
	m_velocity = 0;
}

Physics::~Physics()
{

}

void Physics::Initialize()
{
}

void Physics::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

	m_velocity += elapsedTime;
	if (m_velocity > MAX_GRAVITY)
	{
		m_velocity = MAX_GRAVITY;
	}
	GetOwner()->SetVelocity(GetOwner()->GetVelocity() - Vector3(0, m_velocity, 0));

}


void Physics::Finalize()
{

}