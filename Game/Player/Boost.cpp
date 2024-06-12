#include "pch.h"
#include "Boost.h"
#include "Idol.h"
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
}

void Boost::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	m_player->SetVelocity(m_player->GetVelocity() + Vector3(BOOSTPOWER * elapsedTime,0, 0));

	const auto& kbState = InputManager::GetInstance()->GetKeyboardState();
	const auto& kbTracker = InputManager::GetInstance()->GetKeyboardTracker();
	if (kbState.C)
	{
		m_player->GetComponent<Gravity>().lock().get()->Reset();
	}
	if ( m_player->GetPosition().y + m_player->GetVelocity().y < 0)
	{
		//m_player->ChangeState(m_player->GetIdol());
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
}