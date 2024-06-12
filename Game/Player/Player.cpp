#include "pch.h"
#include "Player.h"
#include "Game/Player/Idol.h"
#include "Game/Player/Jump.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Components/Gravity.h"
#include "Game/Camera.h"
#include "Game/Player/Emitter.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/BodyTop.h"
#include "Game/Parts/BodyBottom.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"
#include "Game/Particle/Shader.h"
#include "Game/Player/Jump.h"
#include "Game/Player/Boost.h"

Player::Player()
{
	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<BoxCollider>();
	AddComponent<Gravity>();
	AddComponent<Emitter>();
	AddPart<Head>();
	AddPart<BodyTop>();
	AddPart<BodyBottom>();
	AddPart<LeftArm>();
	AddPart<RightArm>();
	AddPart<LeftLeg>();
	AddPart<RightLeg>();

	m_idol = std::make_unique<Idol>();
	m_jump = std::make_unique<Jump>();
	m_boost = std::make_unique<Boost>();
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

	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	m_state->Update(elapsedTime);

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


	GetComponent<Emitter>().lock().get()->Render(GetPosition());
	GetPart<Head>().lock().get()->Render(GetWorld());
	GetPart<BodyTop>().lock().get()->Render(GetWorld());
	GetPart<BodyBottom>().lock().get()->Render(GetWorld());
	GetPart<LeftArm>().lock().get()->Render(GetWorld());
	GetPart<RightArm>().lock().get()->Render(GetWorld());
	GetPart<LeftLeg>().lock().get()->Render(GetWorld());
	GetPart<RightLeg>().lock().get()->Render(GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Player::Finalize()
{
	
}

void Player::SetTarget(GameObject* target)
{
	m_target = target;
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