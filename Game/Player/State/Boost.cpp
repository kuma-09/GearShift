#include "pch.h"
#include "Boost.h"
#include "Idol.h"
#include "Jump.h"
#include "Framework/Audio.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/Camera.h"
#include "Game/PlayScene.h"


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
	m_player->GetEnergyGage()->UseEnergyPoint(1);
	m_player->GetComponent<Camera>()->shake();
	static_cast<PlayScene*>(m_player->GetScene())->SetNoise();
	Audio::GetInstance()->PlaySoundSE_Boost();
}

void Boost::Update(float elapsedTime)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	m_totalTime -= elapsedTime;
	
	const auto& mouse = InputManager::GetInstance()->GetMouseState();
	const auto& gpState = InputManager::GetInstance()->GetGamePadState();
	
	m_player->SetVelocity(Vector3(m_velocity.x * m_boostPower * m_totalTime,0, m_velocity.y * m_boostPower * m_totalTime));
	static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetWorld(), m_player->GetQuaternion());
	m_player->GetEnergyGage()->UseEnergyPoint(1);
	if (m_totalTime <= m_boostTime / 2)
	{
		m_player->GetComponent<Gravity>()->Reset();
		m_player->ChangeState(m_player->GetIdol());
	}
	if (mouse.rightButton || gpState.buttons.b)
	{
		if (m_player->GetBoostPoint() > 0)
		{
			m_totalTime = m_boostTime;
		}
	}
	
	m_world =  Matrix::CreateScale(0.5f);
	m_world *= Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(Vector3(XMConvertToRadians(90), 0, 0)) * m_player->GetQuaternion());
	m_world *= Matrix::CreateTranslation(m_player->GetPosition() + Vector3(0,0.5f,0) + Vector3::Transform(Vector3{ 0,0,2.f },m_player->GetQuaternion()));

}

void Boost::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto state = Graphics::GetInstance()->GetCommonStates();
	auto view = Graphics::GetInstance()->GetViewMatrix();
	auto proj = Graphics::GetInstance()->GetProjectionMatrix();

	Resources::GetInstance()->GetEntyuModel()->Draw(context, *state, DirectX::SimpleMath::Matrix::Identity, view, proj, false, [&]
		{
			m_shader->RenderStart(m_world, view, proj);
		}
	);
}

void Boost::Finalize()
{
	
}