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

	m_idol = std::make_unique<Idol>();
	m_jump = std::make_unique<Jump>();
	m_boost = std::make_unique<Boost>();

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
	m_idol->Initialize(this);
	m_jump->Initialize(this);
	m_boost->Initialize(this);
	for (int i = 0; i < MAX_BULLET_CUNT; i++)
	{
		m_bullet[i]->Initalize(this);
	}

	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
	GetComponent<BoxCollider>().lock().get()->SetSize({ 1,1,1 });

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



	GetComponent<Emitter>().lock().get()->Render(GetPosition() - DirectX::SimpleMath::Vector3(0, 1,0));
	//GetComponent<BoxCollider>().lock().get()->Render();
	RenderParts();

}

void Player::Finalize()
{

}

void Player::SetTarget(GameObject* target)
{
	m_target = target;
	GetComponent<Look>().lock().get()->SetTarget(this, target);
	GetComponent<Camera>().lock().get()->SetTarget(this, target);
}

void Player::ChangeState(State* state)
{
	state->SetPosition(m_state->GetPosition());
	state->SetQuaternion(m_state->GetQuaternion());
	state->SetRotation(m_state->GetRotation());
	state->SetScale(m_state->GetScale());
	state->SetVelocity(m_state->GetVelocity());
	state->SetWorld(m_state->GetWorld());
	m_state = state;
	m_state->SetTarget(m_target);
}

void Player::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		SetHP(GetHP() - 1);
	}
}