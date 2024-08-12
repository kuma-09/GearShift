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
	m_player->GetComponent<Gravity>()->Reset();
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

}

void Jump::Render()
{
	
}

void Jump::Finalize()
{
	
}