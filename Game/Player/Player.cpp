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
		m_defaultBullet.push_back(std::make_unique<NormalBullet>(GetScene(), BoxCollider::TypeID::PlayerBullet));
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


}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& gp = m_inputManager->GetGamePadTracker();
	auto& mouse = m_inputManager->GetMouseTracker();
	auto& kb = m_inputManager->GetKeyboardTracker();


	if (mouse->leftButton == mouse->PRESSED || gp->x == gp->PRESSED )
	{
		Shot();
	}

	if (kb->IsKeyPressed(DirectX::Keyboard::R))
	{
		Reload();
	}



	ComponentsUpdate(elapsedTime);

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

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_defaultBullet[i]->Render();
	}

	RenderParts();

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
		for (auto& bullet: m_exBullet)
		{
			if (bullet->GetState() == Bullet::BulletState::UNUSED && m_target)
			{
				bullet->Shot(m_target);
				Audio::GetInstance()->PlaySoundSE_Rocket();
				break;
			}
		}
	}
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
		collider->GetTypeID() == BoxCollider::Wall  ||
		collider->GetTypeID() == BoxCollider::Enemy)
	{
		SetOnFloor(true);
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
