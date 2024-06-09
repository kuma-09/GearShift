#include "pch.h"
#include "Gravity.h"
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

	Vector3 position = GetOwner()->GetPosition();

	if (GetOwner()->GetPosition().y > 0)
	{
		m_velocity += elapsedTime;
		GetOwner()->SetPosition(position - Vector3(0, m_velocity, 0));
	}
	else
	{
		m_velocity = 0;
	}
}


void Gravity::Finalize()
{

}