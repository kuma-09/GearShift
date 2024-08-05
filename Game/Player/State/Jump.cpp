#include "pch.h"
#include "Jump.h"
#include "Idol.h"
#include "Boost.h"
#include "Attack.h"
#include "Game/Components/Gravity.h"


Jump::Jump(Player* player)
{
	m_player = player;
}

Jump::~Jump()
{

}

void Jump::Initialize()
{

}

void Jump::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_player->SetVelocity(m_player->GetVelocity() + Vector3(0, JUMPPOWER * elapsedTime, 0));

	const auto& kbState = InputManager::GetInstance()->GetKeyboardState();

	if (kbState.Space)
	{
		m_player->GetComponent<Gravity>()->Reset();
	}
	if (kbState.V)
	{
		m_player->ChangeState(m_player->GetBoost());
	}
	if ( m_player->GetPosition().y + m_player->GetVelocity().y < 0)
	{
		m_player->ChangeState(m_player->GetIdol());
	}
}

void Jump::Render()
{
	
}

void Jump::Finalize()
{
	
}