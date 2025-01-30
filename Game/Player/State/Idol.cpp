#include "pch.h"
#include "Idol.h"
#include "Jump.h"
#include "Boost.h"
#include "Attack.h"

#include "Game/Components/Physics.h"



Idol::Idol(Player* player)
{
	m_player = player;
}

Idol::~Idol()
{

}

void Idol::Initialize()
{
	m_player->GetComponent<Physics>()->Reset();
}

void Idol::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(elapsedTime);

	const auto& kb = InputManager::GetInstance()->GetKeyboardTracker();
	const auto& mouse = InputManager::GetInstance()->GetMouseTracker();
	//const auto& gpState = InputManager::GetInstance()->GetGamePadState();
	const auto& gpTracker = InputManager::GetInstance()->GetGamePadTracker();

	m_player->GetEnergyGage()->SetEnergyPoint(m_player->GetEnergyGage()->GetEnergyPoint() + 0.5f);

	if (kb->IsKeyPressed(DirectX::Keyboard::Space) || gpTracker->a == gpTracker->PRESSED)
	{
		if (m_player->GetBoostPoint())
		{
			m_player->ChangeState(m_player->GetJump());
		}
		
	}
	if (mouse->rightButton == mouse->PRESSED || gpTracker->b == gpTracker->PRESSED)
	{
		if (m_player->GetBoostPoint())
		{
			m_player->ChangeState(m_player->GetBoost());
		}
	}
}

void Idol::Render()
{

}

void Idol::Finalize()
{
	
}