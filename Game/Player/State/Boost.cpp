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
	m_velocity = { m_player->GetVelocity().x,m_player->GetVelocity().z };
	m_velocity.Normalize();
	m_player->GetEnergyGage()->UseEnergyPoint(1);
}

void Boost::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_totalTime -= elapsedTime;
	
	const auto& mouse = InputManager::GetInstance()->GetMouseState();
	const auto& gpState = InputManager::GetInstance()->GetGamePadState();
	
	m_player->SetVelocity(Vector3(m_velocity.x * m_boostPower * m_totalTime,0, m_velocity.y * m_boostPower * m_totalTime));
	static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetWorld(), m_player->GetQuaternion());
	m_player->GetEnergyGage()->UseEnergyPoint(1);
	if (m_totalTime <= m_boostTime / 2)
	{
		m_player->GetComponent<Gravity>()->Reset();
		m_player->ChangeState(m_player->GetIdol());
	}
	if (mouse.rightButton || gpState.buttons.b)
	{
		if (m_player->GetBoostPoint() > 0)
		{
			m_player->ChangeState(m_player->GetBoost());
		}
	}
	

}

void Boost::Render()
{
	
}

void Boost::Finalize()
{
	
}