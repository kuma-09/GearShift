#include "pch.h"
#include "Attack.h"
#include "Boost.h"
#include "Idol.h"
#include "Jump.h"
#include "Game/Components/Collider.h"
#include "Game/Object/Camera/Camera.h"
#include "Game/PlayScene.h"
#include "Framework/Audio.h"

Attack::Attack(Player* player)
{
	m_player = player;
	m_totalTime = m_boostTime;
	m_sword = std::make_unique<Sword>(m_player->GetScene(), Collider::PlayerSword);
	m_shader = std::make_unique<Shader>();
	m_shader->CreateShader();
}

Attack::~Attack()
{

}

void Attack::Initialize()
{
	using namespace DirectX::SimpleMath;

	m_totalTime = 0;
	m_sword->Initalize(m_player);
	m_player->GetEnergyGage()->UseEnergyPoint(1);
	static_cast<Camera*>(m_player->GetCamera())->shake();
	static_cast<PlayScene*>(m_player->GetScene())->SetNoise();
	Audio::GetInstance()->PlaySoundSE_Boost();
}

void Attack::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_totalTime += elapsedTime;
	
	const auto& mouse = InputManager::GetInstance()->GetMouseTracker();

	m_quaternion = m_player->GetQuaternion();
	m_velocity = Vector3::Transform(Vector3::Forward, m_quaternion);
	float boostSpeed = std::min(m_boostPower * m_totalTime, 1.0f);
	if (m_sword->GetState() == Sword::USED)
	{
		m_player->SetVelocity(Vector3::Lerp(m_player->GetVelocity(), {0,0 ,0}, 0.75f));
	}
	else
	{
		static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetPosition(),0.25f);
		m_player->SetVelocity(Vector3(m_velocity.x * boostSpeed, m_velocity.y * boostSpeed, m_velocity.z * boostSpeed));
	}

	if (m_totalTime >= m_boostTime)
	{
		m_sword->SetState(Sword::USED);
	}

	m_player->GetEnergyGage()->UseEnergyPoint(0.5f);
	m_sword->Update(elapsedTime);

	if (mouse->PRESSED == mouse->rightButton)
	{
		m_player->ChangeState(m_player->GetBoost());
	}

	if (m_sword->GetIsHit())
	{
		m_player->ChangeState(m_player->GetIdol());
	}
}

void Attack::Render()
{
}

void Attack::Finalize()
{
}