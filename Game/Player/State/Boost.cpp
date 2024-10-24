#include "pch.h"
#include "Boost.h"
#include "Idol.h"
#include "Jump.h"
#include "Game/Components/Gravity.h"
#include "Game/PlayScene.h"


Boost::Boost(Player* player)
{
	m_player = player;
	m_totalTime = m_boostTime;
}

Boost::~Boost()
{

}

void Boost::Initialize()
{
	m_totalTime = m_boostTime;
	m_velocity = m_player->GetVelocity();
}

void Boost::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_totalTime -= elapsedTime;
	
	m_player->SetVelocity(Vector3(m_velocity.x * m_boostPower * m_totalTime,0, m_velocity.z * m_boostPower * m_totalTime));
	static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetWorld(), m_player->GetQuaternion());

	if (m_totalTime <= m_boostTime / 2)
	{
		m_player->GetComponent<Gravity>()->Reset();
		m_player->ChangeState(m_player->GetIdol());
	}

}

void Boost::Render()
{
	
}

void Boost::Finalize()
{
	
}