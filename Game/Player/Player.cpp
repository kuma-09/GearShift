#include "pch.h"
#include "Player.h"
#include "Game/Game.h"

#include "Game/Components/HP.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Look.h"
#include "Game/Components/Trail.h"

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


Player::Player(IScene* scene)
	:m_target{}
{
	SetScene(scene);

	m_inputManager = InputManager::GetInstance();

	AddComponent<HP>();
	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<Look>();
	AddComponent<BoxCollider>();
	AddComponent<Gravity>();
	AddComponent<HPBar>();
	AddComponent<Trail>();

	SetPart(Part::Head, std::make_unique<Head>());
	SetPart(Part::BodyTop, std::make_unique<BodyTop>());
	SetPart(Part::LeftArm, std::make_unique<LeftArm>());
	SetPart(Part::RightArm, std::make_unique<RightArm>());
	SetPart(Part::LeftLeg, std::make_unique<LeftLeg>());
	SetPart(Part::RightLeg, std::make_unique<RightLeg>());

	m_idol = std::make_unique<Idol>(this);
	m_jump = std::make_unique<Jump>(this);
	m_boost = std::make_unique<Boost>(this);
	m_attack = std::make_unique<Attack>(this);


	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet.emplace_back(std::make_unique<NormalBullet>(GetScene(), BoxCollider::TypeID::PlayerBullet));
	}

	m_exBullet.clear();

	m_state = m_idol.get();
}

Player::~Player()
{
	Finalize();
}

void Player::Initialize()
{
	for (auto& bullet: m_defaultBullet)
	{
		bullet->Initalize(this);
	}

	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Player);
	GetComponent<BoxCollider>()->SetSize({ 1,1.45f,1 });
	GetComponent<Look>()->SetTarget(this, nullptr);
	GetComponent<Camera>()->SetTarget(this, nullptr);
	GetComponent<HP>()->SetHP(10);
	GetComponent<HPBar>()->Initialize();
	GetComponent<Trail>()->Initialize(L"Resources/Texture/green.png", 20);
	SetOnFloor(false);

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
	if (mouseState.leftButton || gp->x == gp->PRESSED )
	{
		if (m_bulletInterval >= INTERVAL)
		{
			m_bulletInterval = 0;
			Shot();
		}
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::Z) || gp->x == gp->PRESSED)
	{
		ChangeState(GetAttack());
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::R))
	{
		Reload();
	}

	ComponentsUpdate(elapsedTime);
	UpdateParts(elapsedTime);


	Vector3 velocity = GetComponent<Move>()->GetVelocity();
	Quaternion quaternion = Quaternion::CreateFromYawPitchRoll({ 0,0,-velocity.x / 5});
	if (!m_target)
	{
		SetQuaternion(Quaternion::CreateFromYawPitchRoll(GetQuaternion().ToEuler().y, GetQuaternion().ToEuler().x, quaternion.ToEuler().z));
	}
	if (GetOnFloor())
	{
		SetVelocity({ GetVelocity().x, 0, GetVelocity().z });
	}

	m_state->Update(elapsedTime);

	for (auto& bullet: m_defaultBullet)
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

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
	SetOnFloor(false);

	Vector3 pos = { world._41,world._42,world._43 };
	GetComponent<Trail>()->SetPos(pos, pos + Vector3{ 0, -1, 0 });

}

void Player::CreateShadow()
{
	CreateShadows();
}

void Player::Render()
{


	for (auto& bullet : m_defaultBullet)
	{
		bullet->Render();
	}
	if (!m_exBullet.empty())
	{
		for (auto& bullet : m_exBullet)
		{
			bullet->Render();
		}
	}

	RenderParts();

	GetComponent<HPBar>()->Render(GetPosition());
	//GetComponent<Trail>()->Render();

}

void Player::RenderState()
{
	m_state->Render();
}

void Player::RenderPlayerUI()
{
	if (GetBulletSize() == 0 && GetExBulletSize() == 0) m_reload->Render();
	m_energyGage->Render();
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
	int usedCount = 0;
	for (auto& bullet : m_exBullet)
	{
		// “ÁŽê’e‚ð”­ŽË
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
	// “ÁŽê’e‚ª–³‚¢ê‡’Êí’e‚ð”­ŽË‚·‚é
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
	return m_exBulletSize;
}

void Player::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		if (m_state != m_boost.get())
		{
			//GetPart(Part::Head)->Collision(collider);
			//GetPart(Part::BodyTop)->Collision(collider);
			//GetPart(Part::LeftArm)->Collision(collider);
			//GetPart(Part::RightArm)->Collision(collider);
			//GetPart(Part::LeftLeg)->Collision(collider);
			//GetPart(Part::RightLeg)->Collision(collider);
			GetComponent<Camera>()->shake();
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			//collider->Hit();
			static_cast<PlayScene*>(GetScene())->SetNoise();
			if (GetComponent<HP>()->GetHP() <= 0)
			{
				auto game = static_cast<PlayScene*>(GetScene())->GetGame();
				game->ChangeScene(game->GetGameOverScene());
			}
		}
	}
	
	if (collider->GetTypeID() == BoxCollider::Floor)
	{
		SetOnFloor(true);
		//m_boostGage->SetEnergyPoint(0.5f);
		BoxCollider::CheckHit(this, collider->GetOwner());
	}
	if (collider->GetTypeID() == BoxCollider::Wall)
	{
		//m_boostGage->SetEnergyPoint(0.5f);
		BoxCollider::CheckHit(this, collider->GetOwner());
	}

}

void Player::Reload()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet[i]->Initalize(this);
	}
	static_cast<PlayScene*>(GetScene())->UpdateBulletMagazine();
}
