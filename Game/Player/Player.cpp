#include "pch.h"
#include "Player.h"
#include "Game/Game.h"

#include "Game/Components/HP.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Physics.h"
#include "Game/Components/Camera.h"
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
	AddComponent<Camera>();
	AddComponent<Look>();
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
	AddComponent<Trail>();
	AddComponent<PointLight>();

	// ステートの作成
	CreateState();
	// パーツのセット
	CreateDefaultParts();
	// 弾の作成
	CreateBullets();
}

Player::~Player()
{
	Finalize();
}

void Player::Initialize()
{
	GetComponent<HP>()->Initialize(10);
	GetComponent<Move>()->Initialize();
	GetComponent<Camera>()->Initialize();
	GetComponent<Camera>()->SetTarget(this, nullptr);
	GetComponent<Look>()->SetTarget(this, nullptr);
	GetComponent<Physics>()->Initialize();
	GetComponent<Collider>()->Initialize(Collider::Player, { 1,1.45f,1 });
	GetComponent<Collider>()->SetActive(true);
	GetComponent<HPBar>()->Initialize();

	m_burner = std::make_unique<Burner>();
	m_burner->Initialize();

	m_energyGage = std::make_unique<EnergyGage>();
	m_energyGage->Initialize();

	m_reload = std::make_unique<ReloadUI>();
	m_reload->Initialize();

	m_bulletInterval = INTERVAL;
	m_exBulletSize = 0;
}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& gp = m_inputManager->GetGamePadTracker();
	auto& mouseState = m_inputManager->GetMouseState();
	auto& kb = m_inputManager->GetKeyboardTracker();

	m_energyGage->Update(elapsedTime);
	m_reload->Update(elapsedTime);

	m_bulletInterval += elapsedTime;

	if (mouseState.leftButton || gp->x == gp->PRESSED)
	{
		Shot();
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::Z) || gp->x == gp->PRESSED)
	{
		ChangeState(GetAttack());
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::R))
	{
		Reload();
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::P))
	{
		GetScene()->GetGame()->ChangeScene(GetScene()->GetGame()->GetResultScene());
	}

	ComponentsUpdate(elapsedTime);
	UpdateParts(elapsedTime);

	m_state->Update(elapsedTime);

	for (auto& bullet : m_defaultBullet)
	{
		bullet->Update(elapsedTime);
	}
	if (!m_exBullet.empty())
	{
		for (auto& bullet : m_exBullet)
		{
			bullet->Update(elapsedTime);
		}
	}

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
	if (GetBulletSize() == 0 && GetExBulletSize() == 0) m_reload->Render();
	m_energyGage->Render();
	GetComponent<HPBar>()->Render({-0.8f,0.9f});
}

void Player::Finalize()
{	
}

void Player::SetTarget(GameObject* target)
{
	m_target = target;
	GetComponent<Camera>()->SetTarget(this, target);
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
	if (m_bulletInterval < INTERVAL)
	{
		return;
	}
	int usedCount = 0;
	m_bulletInterval = 0;
	for (auto& bullet : m_exBullet)
	{
		// 特殊弾を発射
		if (bullet->GetState() == Bullet::BulletState::UNUSED && m_target)
		{
			m_exBulletSize--;
			bullet->Shot(m_target);
			static_cast<PlayScene*>(GetScene())->UpdateBulletMagazine();
			Audio::GetInstance()->PlaySoundSE_Rocket();
			break;
		}
		{
			usedCount++;
		}
	}
	// 特殊弾が無い場合通常弾を発射する
	if (m_exBullet.empty() || m_exBullet.size() == usedCount)
	{
		for (auto& bullet : m_defaultBullet)
		{
			if (bullet->GetState() == Bullet::BulletState::UNUSED && m_target)
			{
				bullet->Shot(m_target);
				static_cast<PlayScene*>(GetScene())->UpdateBulletMagazine();
				Audio::GetInstance()->PlaySoundSE_Rocket();
				break;
			}
		}
	}
}

float Player::GetBoostPoint()
{
	return m_energyGage->GetEnergyPoint();
}

int Player::GetBulletSize()
{
	int value = 0;
	for (auto& bullet: m_defaultBullet)
	{
		if (bullet->GetState() == Bullet::UNUSED)
		{
			value++;
		}
	}
	return value;
}

int Player::GetMaxBulletSize()
{
	return int(m_defaultBullet.size());
}

int Player::GetExBulletSize()
{
	int value = 0;
	for (auto& bullet : m_exBullet)
	{
		if (bullet->GetState() == Bullet::UNUSED)
		{
			value++;
		}
	}
	return value;
}

void Player::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::EnemyBullet)
	{
		Bullet* bullet = static_cast<Bullet*>(collider->GetOwner());
		if (bullet->GetState() == Bullet::FLYING)
		{
			GetComponent<Camera>()->shake();
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
		for (auto& bullet : m_exBullet)
		{
			bullet->Initialize(this);
		}
		static_cast<PlayScene*>(GetScene())->UpdateBulletMagazine();
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
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet[i]->Initialize(this);
	}
	static_cast<PlayScene*>(GetScene())->UpdateBulletMagazine();
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
	// デフォルトの弾を作成
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet.emplace_back(std::make_unique<NormalBullet>(GetScene(), Collider::TypeID::PlayerBullet));
		m_defaultBullet.back()->Initialize(this);
	}

	// ホーミング弾を作成
	for (int i = 0; i < MAX_EXBULLET_COUNT; i++)
	{
		m_exBullet.emplace_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::PlayerBullet));
		m_exBullet.back()->Initialize(this);
	}
}