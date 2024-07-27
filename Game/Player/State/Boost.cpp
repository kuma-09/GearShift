#include "pch.h"
#include "Boost.h"
#include "Idol.h"
#include "Jump.h"
#include "Game/Components/Gravity.h"


Boost::Boost()
{

}

Boost::~Boost()
{

}

void Boost::Initialize(Player* player)
{
	m_player = player;
	m_totalTime = m_boostTime;
}

void Boost::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_totalTime -= elapsedTime;
	
	m_player->SetVelocity(Vector3(m_velocity.x * m_boostPower * m_totalTime,0, m_velocity.z * m_boostPower * m_totalTime));

	if (m_totalTime <= 0)
	{
		m_player->ChangeState(m_player->GetJump());
	}

}

void Boost::Render()
{
	
}

void Boost::Finalize()
{
	
}

void Boost::SetTarget(GameObject* target)
{
	m_target = target;
	m_totalTime = m_boostTime;
	m_velocity = m_player->GetVelocity();
}