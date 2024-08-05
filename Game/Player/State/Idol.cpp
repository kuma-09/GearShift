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

	if (kb->IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_player->ChangeState(m_player->GetJump());
	}
	if (kb->IsKeyPressed(DirectX::Keyboard::V))
	{
		m_player->ChangeState(m_player->GetBoost());
	}
}

void Idol::Render()
{

}

void Idol::Finalize()
{
	
}