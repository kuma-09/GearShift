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
#include "Game/Components/InputComponent.h"

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
#include "Game/Shader/PostProcess/Noise.h"
#include "Game/PlayScene.h"

#include "BoostGage.h"

#include "Game/Shader/ShadowMap.h"


Player::Player(IScene* scene)
	:m_target{}
{
	SetScene(scene);

	m_inputManager = InputManager::GetInstance();

	AddComponent<HP>();
	AddComponent<InputComponent>();
	AddComponent<Move>();
	AddComponent<Look>();
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
	AddComponent<Trail>();
	AddComponent<Emitter>();
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
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/bolt.png",0.5f,0.05f,0.5f);

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

	m_bulletType = 0;
}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& gp = m_inputManager->GetGamePadTracker();
	auto& mouseState = m_inputManager->GetMouseState();
	auto& mouseTracker = m_inputManager->GetMouseTracker();
	auto& kb = m_inputManager->GetKeyboardTracker();

	m_bulletMagazine->Update(elapsedTime,m_gun->GetMagazineSize());
	m_exBulletMagazine->Update(elapsedTime, m_missileLauncher->GetMagazineSize());
	m_energyGage->Update(elapsedTime);
	m_reload->Update(elapsedTime);
	m_camera->Update(elapsedTime);

	// 弾を発射
	if (mouseState.leftButton) Shot();
	// 近接攻撃
	if (kb->IsKeyPressed(DirectX::Keyboard::Z)) ChangeState(GetAttack());
	// リロード
	if (kb->IsKeyPressed(DirectX::Keyboard::R)) m_gun->Reload();

	// マウスホイールで弾の種類を変更
	m_bulletType = abs(mouseState.scrollWheelValue) / 120;

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
	m_burner->Update(elapsedTime, m_trailPosition, GetQuaternion());
	GetComponent<Emitter>()->SetParticle(GetPosition() + Vector3(rand() % 3 - 1.5f, rand() % 3 - 1.5f, rand() % 3 - 1.5f));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Player::RenderPlayerUI()
{
	if (m_gun->GetMagazineSize() == 0) m_reload->CautionRender();
	m_reload->Render(m_gun->GetNowReloadTime());
	m_bulletMagazine->Render(m_bulletType % 2 == 0 ? true : false);
	m_exBulletMagazine->Render(m_bulletType % 2 == 1 ? true : false);
	m_energyGage->Render();
	GetComponent<HPBar>()->Render({-0.8f,-0.9f});
}

void Player::Finalize()
{	
}

// ターゲットを変更
void Player::SetTarget(GameObject* target)
{
	m_target = target;
	m_camera->SetTarget(this, target);
	GetComponent<Look>()->SetTarget(this, target);
}

// トレイルの座標を変更
void Player::SetTrailPosition(DirectX::SimpleMath::Vector3 pos)
{
	m_trailPosition = pos;
}

// トレイルをクリア
void Player::ClearTrail()
{
	m_burner->ClearTrail();
}

// ステートを変更
void Player::ChangeState(State* state)
{
	m_state->Finalize();
	m_state = state;
	m_state->Initialize();
}

// 弾を発射
void Player::Shot()
{
	switch (m_bulletType % 2)
	{
	case 0:
		m_gun->Shot(m_target);
		m_bulletMagazine->Initialize(m_gun->GetMagazineSize());
		break;
	case 1:
		m_missileLauncher->Shot(m_target);
		m_exBulletMagazine->Initialize(m_missileLauncher->GetMagazineSize());
		break;
	default:
		break;
	}
}

// 残りエネルギー量を取得
float Player::GetBoostPoint()
{
	return m_energyGage->GetEnergyPoint();
}

// 当たり判定
void Player::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::EnemyBullet)
	{
		Bullet* bullet = static_cast<Bullet*>(collider->GetOwner());
		if (bullet->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			bullet->Hit();
			Noise::SetHitNoise(true);
			static_cast<Camera*>(GetCamera())->shake();
			if (GetComponent<HP>()->GetHP() <= 0)
			{
				auto game = static_cast<PlayScene*>(GetScene())->GetGame();
				static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
				game->ChangeScene(game->GetGameOverScene());
			}
		}
	}

	if (collider->GetTypeID() == Collider::DropItem)
	{
		GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() + 5);
		Audio::GetInstance()->PlaySoundSE_PowerUp();
	}

	if (collider->GetTypeID() == Collider::DropItemB)
	{
		m_missileLauncher->Reload();
		Audio::GetInstance()->PlaySoundSE_PowerUp();
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
	// 残弾数表示用UIの初期化
	m_exBulletMagazine = std::make_unique<ExBulletMagazine>();
	m_exBulletMagazine->Initialize(m_missileLauncher->GetMagazineSize());
}