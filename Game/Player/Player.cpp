#include "pch.h"
#include "Player.h"
#include "Game/Player/State/Idol.h"
#include "Game/Player/State/Jump.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Components/Camera.h"
#include "Game/Components/Look.h"
#include "Game/Particle/Emitter.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"
#include "Game/Particle/Shader.h"
#include "Game/Player/State/Jump.h"
#include "Game/Player/State/Boost.h"
#include "Game/Player/State/Attack.h"
#include "Game/Object/Bullet.h"



Player::Player(IScene* scene)
	:m_target{}
{
	SetScene(scene);

	m_inputManager = InputManager::GetInstance();

	AddComponent<Look>();
	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<BoxCollider>();
	AddComponent<Gravity>();
	AddComponent<Emitter>();

	m_idol = std::make_unique<Idol>(this);
	m_jump = std::make_unique<Jump>(this);
	m_boost = std::make_unique<Boost>(this);
	m_attack = std::make_unique<Attack>(this);
	

	for (int i = 0; i < MAX_BULLET_CUNT; i++)
	{
		m_bullet[i] = std::make_unique<Bullet>(GetScene(), BoxCollider::TypeID::PlayerBullet);
	}

	m_state = m_idol.get();
}

Player::~Player()
{

}

void Player::Initialize()
{
	for (int i = 0; i < MAX_BULLET_CUNT; i++)
	{
		m_bullet[i]->Initalize(this);
	}

	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Player);
	GetComponent<BoxCollider>()->SetSize({ 1,1,1 });

}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	auto& kb = m_inputManager->GetKeyboardTracker();
	auto& gp = m_inputManager->GetGamePadTracker();

	if (kb->IsKeyPressed(DirectX::Keyboard::C) || gp->x == gp->PRESSED )
	{
		for (int i = 0; i < MAX_BULLET_CUNT; i++)
		{
			if (m_bullet[i]->GetState() == Bullet::BulletState::UNUSED)
			{
				m_bullet[i]->Shot(this);
				break;
			}
		}
	}

	ComponentsUpdate(elapsedTime);
	UpdateParts(elapsedTime);

	m_state->Update(elapsedTime);

	if (m_state == m_attack.get())
	{
		GetComponent<BoxCollider>()->SetSize(Vector3(2.f,2.f,2.f));
	}
	else
	{
		GetComponent<BoxCollider>()->SetSize(Vector3(1.f,1.f,1.f));
	}

	for (int i = 0; i < MAX_BULLET_CUNT; i++)
	{
		m_bullet[i]->Update(elapsedTime);
	}


	SetPosition(GetPosition() + GetVelocity());

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Player::Render()
{
	m_state->Render();

	for (int i = 0; i < MAX_BULLET_CUNT; i++)
	{
		m_bullet[i]->Render();
	}


	GetComponent<Emitter>()->Render(GetPosition() - DirectX::SimpleMath::Vector3(0, 1,0));
	GetComponent<BoxCollider>()->Render();
	RenderParts();

}

void Player::Finalize()
{

}

void Player::SetTarget(GameObject* target)
{
	m_target = target;
	GetComponent<Look>()->SetTarget(this, target);
	GetComponent<Camera>()->SetTarget(this, target);
}

void Player::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void Player::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		if (m_state != m_boost.get())
		{
			SetHP(GetHP() - 1);
			GetComponent<Camera>()->shake();
		}
	}
}