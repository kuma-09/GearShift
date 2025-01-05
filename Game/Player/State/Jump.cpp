#include "pch.h"
#include "Jump.h"
#include "Idol.h"
#include "Boost.h"
#include "Attack.h"
#include "Game/Components/Physics.h"
#include "Game/PlayScene.h"


Jump::Jump(Player* player)
{
	m_player = player;
}

Jump::~Jump()
{

}

void Jump::Initialize()
{
	m_player->GetComponent<Physics>()->Reset();
}

void Jump::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_player->SetVelocity(m_player->GetVelocity() + Vector3(0, JUMPPOWER * elapsedTime, 0));

	const auto& kbState = InputManager::GetInstance()->GetKeyboardState();
	const auto& mouse = InputManager::GetInstance()->GetMouseTracker();
	const auto& gpState = InputManager::GetInstance()->GetGamePadState();
	const auto& gpTracker = InputManager::GetInstance()->GetGamePadTracker();
	static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetPosition(), 0.25f);	if (m_player->GetVelocity().y < 0)
	{
		m_player->GetComponent<Physics>()->Reset();
		m_player->ChangeState(m_player->GetIdol());
	}
	if (kbState.Space || gpState.buttons.a)
	{
		if (m_player->GetBoostPoint() > 0)
		{
			m_player->GetEnergyGage()->UseEnergyPoint(1);
			m_player->GetComponent<Physics>()->Reset();
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

void Jump::Render()
{
	
}

void Jump::Finalize()
{
	
}