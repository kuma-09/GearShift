#include "pch.h"
#include "Gravity.h"
#include "BoxCollider.h"
#include "Game/GameObject.h"


Gravity::Gravity()
{
	m_velocity = 0;
}

Gravity::~Gravity()
{

}

void Gravity::Initialize()
{
}

void Gravity::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

	m_velocity += elapsedTime;
	if (m_velocity > MAX_GRAVITY)
	{
		m_velocity = MAX_GRAVITY;
	}
	GetOwner()->SetVelocity(GetOwner()->GetVelocity() - Vector3(0, m_velocity, 0));
	

}


void Gravity::Finalize()
{

}