#include "pch.h"
#include "Player.h"

#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Look.h"


#include "Game/Player/State/Idol.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Boost.h"
#include "Game/Player/State/Attack.h"
#include "Game/Object/Bullet/NormalBullet.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include <Game/Components/HP.h>

#include "Framework/Audio.h"
#include "Game/PlayScene.h"

#include "BoostGage.h"
#include "BulletGage.h"


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
	GetComponent<BoxCollider>()->SetSize({ 1,1.25f,1 });
	GetComponent<Look>()->SetTarget(this, nullptr);
	GetComponent<Camera>()->SetTarget(this, nullptr);
	GetComponent<HP>()->SetHP(100);
	GetComponent<HPBar>()->Initialize();
	SetOnFloor(false);

	m_boostGage = std::make_unique<BoostGage>();
	m_boostGage->Initialize();

	m_bulletGage = std::make_unique<BulletGage>();
	m_bulletGage->Initialize();
}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& gp = m_inputManager->GetGamePadTracker();
	auto& mouse = m_inputManager->GetMouseTracker();
	auto& kb = m_inputManager->GetKeyboardTracker();

	m_boostGage->Update();

	m_bulletGage->SetBoostPoint(GetBulletSize());
	m_bulletGage->SetMaxBoostPoint(GetMaxBulletSize());

	if (mouse->leftButton == mouse->PRESSED || gp->x == gp->PRESSED )
	{
		Shot();
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::R))
	{
		Reload();
	}

	if (dynamic_cast<Idol*>(m_state))
	{
		m_boostGage->SetBoostPoint(0.5f);
	}

	ComponentsUpdate(elapsedTime);

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

	UpdateParts(elapsedTime);

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

}

void Player::CreateShadow()
{
	CreateShadows();
}

void Player::Render()
{
	m_state->Render();

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
	//m_boostGage->Render();
	m_bulletGage->Render();
	//GetComponent<BoxCollider>()->Render();

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

	if (m_exBullet.empty())
	{
		for (auto& bullet: m_defaultBullet)
		{
			if (bullet->GetState() == Bullet::BulletState::UNUSED && m_target)
			{
				bullet->Shot(m_target);
				Audio::GetInstance()->PlaySoundSE_Rocket();
				break;
			}
		}
	}
	else
	{
		int usedCount = 0;
		for (auto& bullet: m_exBullet)
		{
			if (bullet->GetState() == Bullet::BulletState::UNUSED && m_target)
			{
				bullet->Shot(m_target);
				Audio::GetInstance()->PlaySoundSE_Rocket();
				break;
			}
			{
				usedCount++;
			}
		}
		if (m_exBullet.size() == usedCount)
		{
			for (auto& bullet: m_exBullet)
			{
				bullet.reset();
			}
			m_exBullet.clear();
		}
	}
}

float Player::GetBoostPoint()
{
	return m_boostGage->GetBoostPoint();
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
	return m_defaultBullet.size();
}

void Player::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		if (m_state != m_boost.get())
		{
			GetPart(Part::Head)->Collision(collider);
			GetPart(Part::BodyTop)->Collision(collider);
			GetPart(Part::LeftArm)->Collision(collider);
			GetPart(Part::RightArm)->Collision(collider);
			GetPart(Part::LeftLeg)->Collision(collider);
			GetPart(Part::RightLeg)->Collision(collider);
		}
	}
	
	if (collider->GetTypeID() == BoxCollider::Floor ||
		collider->GetTypeID() == BoxCollider::Wall)
	{
		SetOnFloor(true);
		m_boostGage->SetBoostPoint(0.5f);
		BoxCollider::CheckHit(this, collider->GetOwner());
	}

	if (collider->GetTypeID() == BoxCollider::DropItem)
	{

	}
}

void Player::Reload()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet[i]->Initalize(this);
	}
}
