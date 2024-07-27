#include "pch.h"
#include "Idol.h"
#include "Jump.h"
#include "Boost.h"



Idol::Idol()
	:m_player{}
	,m_target{}
{
	m_inputManager = InputManager::GetInstance();
}

Idol::~Idol()
{

}

void Idol::Initialize(Player* player)
{
	m_player = player;
}

void Idol::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;


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

void Idol::SetTarget(GameObject* target)
{
	m_target = target;
	
}