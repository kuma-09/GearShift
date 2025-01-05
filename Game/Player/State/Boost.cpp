#include "pch.h"
#include "Boost.h"
#include "Idol.h"
#include "Jump.h"
#include "Framework/Audio.h"
#include "Game/Components/Physics.h"
#include "Game/components/Move.h"
#include "Game/components/PointLight.h"
#include "Game/Object/Camera/Camera.h"
#include "Game/PlayScene.h"

#include "Game/Shader/PostProcess/Noise.h"

Boost::Boost(Player* player)
{
	m_player = player;
	m_totalTime = m_boostTime;
	m_shader = std::make_unique<Shader>();
	m_shader->CreateShader();
	
}

Boost::~Boost()
{

}

void Boost::Initialize()
{
	m_totalTime = m_boostTime;
	m_velocity = { m_player->GetVelocity().x,m_player->GetVelocity().z };
	m_velocity.Normalize();
	m_player->ClearTrail();
	m_player->SetTrailPosition(m_player->GetPosition());
	m_player->GetEnergyGage()->UseEnergyPoint(1);
	m_player->GetComponent<PointLight>()->Initialize(m_player->GetPosition(),{ 0.2f, 0.29f, 0.46f });
	static_cast<Camera*>(m_player->GetCamera())->shake();
	Audio::GetInstance()->PlaySoundSE_Boost();
	Noise::SetNoise(true);
}

void Boost::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_totalTime -= elapsedTime;
	
	const auto& mouse = InputManager::GetInstance()->GetMouseState();
	const auto& gpState = InputManager::GetInstance()->GetGamePadState();
	Vector3 velocity = m_player->GetComponent<Move>()->GetVelocity();
	Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(m_player->GetCamera()->GetQuaternion().ToEuler().y, 0, 0);
	velocity = Vector3::Transform(velocity, quaternion);
	velocity.Normalize();
	m_velocity = Vector2::Lerp(m_velocity, Vector2(velocity.x,velocity.z), 0.1f);
	m_player->SetVelocity(Vector3(m_velocity.x * m_boostPower * m_totalTime,0, m_velocity.y * m_boostPower * m_totalTime));
	static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetPosition(),0.25f);
	m_player->SetTrailPosition(m_player->GetPosition());
	m_player->GetEnergyGage()->UseEnergyPoint(1);
	if (m_totalTime <= m_boostTime / 2)
	{
		m_player->ChangeState(m_player->GetIdol());
	}
	if (mouse.rightButton || gpState.buttons.b)
	{
		if (m_player->GetBoostPoint() > 0)
		{
			m_totalTime = m_boostTime;
		}
	}
}

void Boost::Finalize()
{
	m_player->GetComponent<PointLight>()->ClearColor();
}

