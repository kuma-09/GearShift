#include "pch.h"
#include "Idol.h"
#include "Jump.h"



Idol::Idol()
{

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

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	const auto& kb = InputManager::GetInstance()->GetKeyboardTracker();
	if (kb->IsKeyPressed(DirectX::Keyboard::C))
	{
		m_player->ChangeState(m_player->GetJump());
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