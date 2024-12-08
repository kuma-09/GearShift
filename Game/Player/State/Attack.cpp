#include "pch.h"
#include "Attack.h"
#include "Boost.h"
#include "Idol.h"
#include "Jump.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Camera.h"	
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
	m_player->GetComponent<Camera>()->shake();
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
		static_cast<PlayScene*>(m_player->GetScene())->CreateHitParticle(m_player->GetWorld(), m_player->GetQuaternion());
		m_player->SetVelocity(Vector3(m_velocity.x * boostSpeed, m_velocity.y * boostSpeed, m_velocity.z * boostSpeed));
	}

	if (m_totalTime >= m_boostTime)
	{
		m_sword->SetState(Sword::USED);
	}

	m_player->GetEnergyGage()->UseEnergyPoint(1);
	m_sword->Update(elapsedTime);

	if (mouse->PRESSED == mouse->rightButton)
	{
		m_player->ChangeState(m_player->GetBoost());
	}

	if (m_sword->GetIsHit())
	{
		m_player->ChangeState(m_player->GetIdol());
	}
	m_world = Matrix::CreateScale(0.25f);
	m_world *= Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(Vector3(DirectX::XMConvertToRadians(90), 0, 0)) * m_player->GetQuaternion());
	m_world *= Matrix::CreateTranslation(m_player->GetPosition() + Vector3(0, 0.5f, 0) + Vector3::Transform(Vector3{ 0,0,1.2f }, m_player->GetQuaternion()));
}

void Attack::Render()
{
	m_sword->Render();
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

void Attack::Finalize()
{

}