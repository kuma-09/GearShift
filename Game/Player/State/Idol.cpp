#include "pch.h"
#include "Idol.h"
#include "Jump.h"
#include "Boost.h"
#include "Attack.h"



Idol::Idol(Player* player)
{
	m_player = player;
}

Idol::~Idol()
{

}

void Idol::Initialize()
{

}

void Idol::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	UNREFERENCED_PARAMETER(elapsedTime);

	const auto& kb = InputManager::GetInstance()->GetKeyboardTracker();
	const auto& mouse = InputManager::GetInstance()->GetMouseTracker();
	//const auto& gpState = InputManager::GetInstance()->GetGamePadState();
	const auto& gpTracker = InputManager::GetInstance()->GetGamePadTracker();

	m_player->GetEnergyGage()->SetEnergyPoint(1);

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