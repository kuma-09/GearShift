#include "pch.h"
#include "Player.h"
#include "Game/Game.h"

#include "Game/Components/HP.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Physics.h"
#include "Game/Components/Look.h"
#include "Game/Components/Trail.h"
#include "Game/Components/PointLight.h"

#include "Game/Parts/Part.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"

#include "Game/Player/State/Idol.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Boost.h"
#include "Game/Player/State/Attack.h"
#include "Game/Object/Bullet/NormalBullet.h"
#include "Game/Object/Bullet/HomingBullet.h"

#include "Framework/Audio.h"
#include "Game/PlayScene.h"

#include "BoostGage.h"

#include "Game/Shader/ShadowMap.h"


Player::Player(IScene* scene)
	:m_target{}
{
	SetScene(scene);

	m_inputManager = InputManager::GetInstance();

	AddComponent<HP>();
	AddComponent<Move>();
	AddComponent<Look>();
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
	AddComponent<Trail>();
	AddComponent<PointLight>();
}

Player::~Player()
{
	Finalize();
}

void Player::Initialize()
{
	GetComponent<HP>()->Initialize(10);
	GetComponent<Move>()->Initialize();
	GetComponent<Look>()->SetTarget(this, nullptr);
	GetComponent<Physics>()->Initialize();
	GetComponent<Collider>()->Initialize(Collider::Player, { 1,1.45f,1 });
	GetComponent<Collider>()->SetActive(true);
	GetComponent<HPBar>()->Initialize();

	// ステートの作成
	CreateState();
	// パーツのセット
	CreateDefaultParts();
	// 弾の作成
	CreateBullets();

	m_camera = std::make_unique<Camera>();
	m_camera->Initialize();

	m_burner = std::make_unique<Burner>();
	m_burner->Initialize();

	m_energyGage = std::make_unique<EnergyGage>();
	m_energyGage->Initialize();

	m_reload = std::make_unique<ReloadUI>();
	m_reload->Initialize();

}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& gp = m_inputManager->GetGamePadTracker();
	auto& mouseState = m_inputManager->GetMouseState();
	auto& kb = m_inputManager->GetKeyboardTracker();

	m_energyGage->Update(elapsedTime);
	m_reload->Update(elapsedTime);
	m_camera->Update(elapsedTime);

	// 弾を発射
	if (mouseState.leftButton) Shot();
	// 近接攻撃
	if (kb->IsKeyPressed(DirectX::Keyboard::Z)) ChangeState(GetAttack());
	// リロード
	if (kb->IsKeyPressed(DirectX::Keyboard::R)) Reload();

	SetVelocity(Vector3::Zero);
	ComponentsUpdate(elapsedTime);
	UpdateParts(elapsedTime);
	Vector3 velocity = GetComponent<Move>()->GetVelocity();
	Quaternion quaternion = Quaternion::CreateFromYawPitchRoll(m_camera->GetQuaternion().ToEuler().y, 0, 0);
	SetVelocity(GetVelocity() + Vector3::Transform(velocity, quaternion));
	
	if (!GetTarget() && GetComponent<Move>()->GetIsMove())
	{
		velocity = GetVelocity();
		velocity.x *= -1;
		velocity.y = 0;
		quaternion = Quaternion::CreateFromRotationMatrix(Matrix::CreateLookAt(GetPosition(), GetPosition() + velocity, Vector3::Up));
		SetQuaternion(Quaternion::Lerp(GetQuaternion(), quaternion, 0.1f));
	}


	m_state->Update(elapsedTime);

	// Gunの更新
	m_gun->Update(elapsedTime);

	m_missileLauncher->Update(elapsedTime);

	SetPrePosition(GetPosition());
	SetPosition(GetPosition() + GetVelocity());
	ShadowMap::SetLightPosition(GetPosition());
	m_burner->Update(elapsedTime,GetPosition(),GetQuaternion());


	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Player::CreateShadow()
{
	CreateShadows();
}

void Player::Render()
{
}

void Player::RenderState()
{
	m_state->Render();
}

void Player::RenderPlayerUI()
{
	if (m_gun->GetMagazineSize() == 0) m_reload->Render();
	m_bulletMagazine->Render();
	m_energyGage->Render();
	GetComponent<HPBar>()->Render({-0.8f,0.9f});
}

void Player::Finalize()
{	
}

void Player::SetTarget(GameObject* target)
{
	m_target = target;
	m_camera->SetTarget(this, target);
	GetComponent<Look>()->SetTarget(this, target);
}

void Player::ChangeState(State* state)
{
	m_state->Finalize();
	m_state = state;
	m_state->Initialize();
}

void Player::Shot()
{
	m_gun->Shot(m_target);
	m_missileLauncher->Shot(m_target);
	m_bulletMagazine->Initialize(m_gun->GetMagazineSize());
}

float Player::GetBoostPoint()
{
	return m_energyGage->GetEnergyPoint();
}


void Player::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::EnemyBullet)
	{
		Bullet* bullet = static_cast<Bullet*>(collider->GetOwner());
		if (bullet->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			bullet->Hit();
			static_cast<PlayScene*>(GetScene())->SetNoise();
			if (GetComponent<HP>()->GetHP() <= 0)
			{
				auto game = static_cast<PlayScene*>(GetScene())->GetGame();
				static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
				game->ChangeScene(game->GetGameOverScene());
			}
		}
	}

	if (collider->GetTypeID() == Collider::DropItemB)
	{
		m_missileLauncher->Reload();
	}
	
	if (collider->GetTypeID() == Collider::Floor)
	{
		Collider::CheckHit(this, collider->GetOwner());
		GetPart(Part::LeftLeg)->GetComponent<Emitter>()->SetParticle(GetPart(Part::LeftLeg)->GetPosition() - DirectX::SimpleMath::Vector3{ 0,1.f,0 });
		GetPart(Part::RightLeg)->GetComponent<Emitter>()->SetParticle(GetPart(Part::RightLeg)->GetPosition() - DirectX::SimpleMath::Vector3{ 0,1.f,0 });
	}
	if (collider->GetTypeID() == Collider::Wall || collider->GetTypeID() == Collider::Enemy)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}

}

void Player::Reload()
{
	m_gun->Reload();
}

// プレイヤーのステートを作成
void Player::CreateState()
{
	m_idol = std::make_unique<Idol>(this);
	m_jump = std::make_unique<Jump>(this);
	m_boost = std::make_unique<Boost>(this);
	m_attack = std::make_unique<Attack>(this);
	m_state = m_idol.get();
}

// デフォルトパーツをセット
void Player::CreateDefaultParts()
{
	SetPart(Part::Head,     std::make_unique<Head>());
	SetPart(Part::BodyTop,  std::make_unique<BodyTop>());
	SetPart(Part::LeftArm,  std::make_unique<LeftArm>());
	SetPart(Part::RightArm, std::make_unique<RightArm>());
	SetPart(Part::LeftLeg,  std::make_unique<LeftLeg>());
	SetPart(Part::RightLeg, std::make_unique<RightLeg>());
}

// 初期弾を作成
void Player::CreateBullets()
{
	// Gunの初期化
	m_gun = std::make_unique<Gun>(this);
	m_gun->Initialize();
	// 残弾数表示用UIの初期化
	m_bulletMagazine = std::make_unique<BulletMagazine>();
	m_bulletMagazine->Initialize(m_gun->GetMagazineSize());
	// ミサイルランチャーの初期化
	m_missileLauncher = std::make_unique<MissileLauncher>(this);
	m_missileLauncher->Initialize();
}