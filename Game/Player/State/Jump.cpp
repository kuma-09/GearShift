#include "pch.h"
#include "Jump.h"
#include "Idol.h"
#include "Boost.h"
#include "Game/Components/Gravity.h"


Jump::Jump()
{

}

Jump::~Jump()
{

}

void Jump::Initialize(Player* player)
{
	m_player = player;
}

void Jump::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	m_player->SetVelocity(m_player->GetVelocity() + Vector3(0, JUMPPOWER * elapsedTime, 0));

	const auto& kbState = InputManager::GetInstance()->GetKeyboardState();

	if (kbState.Space)
	{
		m_player->GetComponent<Gravity>().lock().get()->Reset();
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

void Jump::SetTarget(GameObject* target)
{
	m_target = target;
}