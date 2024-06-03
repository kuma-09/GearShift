#include "pch.h"
#include "Player.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/Move.h"
#include "Game/Camera.h"
#include "Game/Parts/Head.h"
#include "Game/Parts/Body.h"
#include "Game/Parts/LeftArm.h"
#include "Game/Parts/LeftLeg.h"
#include "Game/Parts/RightArm.h"
#include "Game/Parts/RightLeg.h"


Player::Player()
{
	AddComponent<Move>();
	AddComponent<Camera>();
	AddComponent<BoxCollider>();
	AddPart<Head>();
	AddPart<Body>();
	AddPart<LeftArm>();
	AddPart<RightArm>();
	AddPart<LeftLeg>();
	AddPart<RightLeg>();
}

Player::~Player()
{

}

void Player::Initialize()
{
	GetComponent<Camera>().lock().get()->SetTarget(this, m_target);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void Player::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);
	PartUpdate(elapsedTime);

	Matrix world = Matrix::Identity;
	world =  Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Player::Render()
{

	GetPart<Head>().lock().get()->Render(GetWorld());
	GetPart<Body>().lock().get()->Render(GetWorld());
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
	GetComponent<Camera>().lock().get()->SetTarget(this, m_target);
}