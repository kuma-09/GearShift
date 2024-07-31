#include "pch.h"
#include "Attack.h"
#include "Idol.h"
#include "Jump.h"
#include "Game/Components/BoxCollider.h"


Attack::Attack(Player* player)
{
	m_player = player;
	m_totalTime = m_boostTime;
}

Attack::~Attack()
{

}

void Attack::Initialize()
{
	using namespace DirectX::SimpleMath;

	m_totalTime = m_boostTime;
	m_quaternion = m_player->GetQuaternion();
	m_velocity = Vector3::Transform(Vector3::Forward, m_quaternion);
	m_sword = std::make_unique<Sword>(m_player->GetScene(),BoxCollider::PlayerBullet);
	m_sword->Initalize(m_player);
}

void Attack::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_totalTime -= elapsedTime;
	
	m_player->SetVelocity(Vector3(m_velocity.x * m_boostPower * m_totalTime, m_velocity.y * m_boostPower * m_totalTime, m_velocity.z * m_boostPower * m_totalTime));

	m_sword->Update(elapsedTime);

	if (m_totalTime <= 0)
	{
		m_sword.release();
		m_player->ChangeState(m_player->GetIdol());
	}

}

void Attack::Render()
{
	m_sword->Render();
}

void Attack::Finalize()
{

}