#include "pch.h"
#include "Player.h"

#include "Game/Components/HPBar.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Look.h"

#include "Game/Particle/Emitter.h"

#include "Game/Player/State/Idol.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Boost.h"
#include "Game/Player/State/Attack.h"
#include "Game/Object/Bullet/NormalBullet.h"
#include "Game/Object/Bullet/HomingBullet.h"



Player::Player(IScene* scene)
	:m_target{}
{
	SetScene(scene);

	m_inputManager = InputManager::GetInstance();

	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<Look>();
	AddComponent<BoxCollider>();
	AddComponent<Gravity>();
	AddComponent<Emitter>();
	AddComponent<HPBar>();

	m_idol = std::make_unique<Idol>(this);
	m_jump = std::make_unique<Jump>(this);
	m_boost = std::make_unique<Boost>(this);
	m_attack = std::make_unique<Attack>(this);
	

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_bullet.push_back(std::make_unique<NormalBullet>(GetScene(), BoxCollider::TypeID::PlayerBullet));
	}

	m_state = m_idol.get();
}

Player::~Player()
{
	Finalize();
}

void Player::Initialize()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_bullet[i]->Initalize(this);
	}

	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Player);
	GetComponent<BoxCollider>()->SetSize({ 1,1.5f,1 });
	GetComponent<Look>()->SetTarget(this, nullptr);
	GetComponent<Camera>()->SetTarget(this, nullptr);
	SetHP(100);
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

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		if (m_bullet[i]->GetState() == Bullet::FLYING)
		{
			m_bullet[i]->Update(elapsedTime);
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

void Player::Render()
{
	m_state->Render();

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_bullet[i]->Render();
	}

	GetComponent<Emitter>()->Render(GetPosition() - DirectX::SimpleMath::Vector3(0, 1,0));
	RenderParts();

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

	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		if (m_bullet[i]->GetState() == Bullet::BulletState::UNUSED && m_target)
		{
			m_bullet[i]->Shot(m_target);
			break;
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
			GetPart(Part::BodyBottom)->Collision(collider);
			GetPart(Part::LeftArm)->Collision(collider);
			GetPart(Part::RightArm)->Collision(collider);
			GetPart(Part::LeftLeg)->Collision(collider);
			GetPart(Part::RightLeg)->Collision(collider);
		}
	}
	
	if (collider->GetTypeID() == BoxCollider::Floor || collider->GetTypeID() == BoxCollider::Wall)
	{
		SetOnFloor(true);
	}
}

void Player::Reload()
{
	for (int i = 0; i < MAX_BULLET_COUNT; i++)
	{
		m_bullet[i]->Initalize(this);
	}
}
