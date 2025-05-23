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


Player::Player(Scene* scene)
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
	GetComponent<Collider>()->Initialize(Collider::Player,Collider::Collision, { 1,1.45f,1 });
	GetComponent<Collider>()->SetActive(true);
	GetComponent<HPBar>()->Initialize();
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/bolt.png",0.5f,0.05f,0.5f);
	GetComponent<Emitter>()->SetState(false, true);

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
	auto& mouseState = m_inputManager->GetMouseState();
	auto& kb = m_inputManager->GetKeyboardTracker();

	m_bulletMagazine->Update(elapsedTime, static_cast<int>(m_gun->GetMagazineSize()));
	m_exBulletMagazine->Update(elapsedTime, static_cast<int>(m_missileLauncher->GetMagazineSize()));
	m_energyGage->Update(elapsedTime);
	m_reload->Update(elapsedTime);
	m_camera->Update(elapsedTime);

	// 弾を発射
	if (mouseState.leftButton) Shot();
	// 近接攻撃
	if (kb->IsKeyPressed(DirectX::Keyboard::F))  SwordAttack();
	// リロード
	if (kb->IsKeyPressed(DirectX::Keyboard::R)) m_gun->Reload();

	// マウスホイールで弾の種類を変更
	m_bulletType = abs(mouseState.scrollWheelValue) / 120;

	SetVelocity(Vector3::Zero);
	ComponentsUpdate(elapsedTime);
	ChildObjectsUpdate(elapsedTime);
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
	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
	ShadowMap::SetLightPosition(GetPosition());
	m_burner->Update(elapsedTime, m_trailPosition, GetQuaternion());
	Vector3 diff = Vector3(rand() % 4 - 1.5f, rand() % 4 - 1.5f, rand() % 4 - 1.5f);
	if (GetComponent<HP>()->GetHP() < 3)
	{
		GetComponent<Emitter>()->SetParticle(GetPosition() + diff);
	}

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
void Player::ChangeState(IState* state)
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
		m_bulletMagazine->Initialize(static_cast<int>(m_gun->GetMagazineSize()));
		break;
	case 1:
		m_missileLauncher->Shot(m_target);
		m_exBulletMagazine->Initialize(static_cast<int>(m_missileLauncher->GetMagazineSize()));
		break;
	default:
		break;
	}
}

void Player::SwordAttack()
{
	if (GetBoostPoint() > 0)
	{
		ChangeState(GetAttack());
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
	if (collider->GetTypeID() == Collider::Floor)
	{
		Collider::CheckHit(this, collider->GetOwner());
		GetChild<LeftLeg>("LeftLeg")->GetComponent<Emitter>()->SetParticle(GetChild<LeftLeg>("LeftLeg")->GetPosition() - DirectX::SimpleMath::Vector3{ 0,1.f,0 });
		GetChild<RightLeg>("RightLeg")->GetComponent<Emitter>()->SetParticle(GetChild<RightLeg>("RightLeg")->GetPosition() - DirectX::SimpleMath::Vector3{ 0,1.f,0 });
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
	SetChild<Head>("Head");
	SetChild<BodyTop>("BodyTop");
	SetChild<LeftArm>("LeftArm");
	SetChild<RightArm>("RightArm");
	SetChild<LeftLeg>("LeftLeg");
	SetChild<RightLeg>("RightLeg");

	GetChild<Head>("Head")->Initialize();
	GetChild<BodyTop>("BodyTop")->Initialize();
	GetChild<LeftArm>("LeftArm")->Initialize();
	GetChild<RightArm>("RightArm")->Initialize();
	GetChild<LeftLeg>("LeftLeg")->Initialize();
	GetChild<RightLeg>("RightLeg")->Initialize();
}

// 初期弾を作成
void Player::CreateBullets()
{
	// Gunの初期化
	m_gun = std::make_unique<Gun>(this);
	m_gun->Initialize();
	// 残弾数表示用UIの初期化
	m_bulletMagazine = std::make_unique<BulletMagazine>();
	m_bulletMagazine->Initialize(static_cast<int>(m_gun->GetMagazineSize()));
	// ミサイルランチャーの初期化
	m_missileLauncher = std::make_unique<MissileLauncher>(this);
	m_missileLauncher->Initialize();
	// 残弾数表示用UIの初期化
	m_exBulletMagazine = std::make_unique<ExBulletMagazine>();
	m_exBulletMagazine->Initialize(static_cast<int>(m_missileLauncher->GetMagazineSize()));
}