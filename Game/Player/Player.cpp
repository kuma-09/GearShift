#include "pch.h"
#include "Player.h"
#include "Game/Camera.h"
#include "Game/Player/Emitter.h"
#include "Game/Player/Idol.h"



Player::Player()
{
	m_idol = std::make_unique<Idol>();

	m_state = m_idol.get();
}

Player::~Player()
{

}

void Player::Initialize()
{
	m_state->Initialize(this);
}

void Player::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	m_state->Update(elapsedTime);
}

void Player::Render()
{
	m_state->Render();
}

void Player::Finalize()
{
	
}

void Player::SetTarget(GameObject* target)
{
	m_state->SetTarget(target);
}