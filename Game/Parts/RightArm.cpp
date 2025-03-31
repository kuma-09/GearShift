#include "pch.h"
#include "RightArm.h"
#include "Game/Game.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

RightArm::RightArm()
{
	AddComponent<ModelDraw>();
	SetTypeID(TypeID::RightArm);
	m_isHit = false;
}

RightArm::~RightArm()
{
	Finalize();
}

void RightArm::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::RArm));
	GetComponent<ModelDraw>()->SetRimLithgColor({ 0.75f,0.75f,1,1 });
}

void RightArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	SetQuaternion(Quaternion::CreateFromYawPitchRoll(0, DirectX::XMConvertToRadians(45), 0) * GetOwner()->GetQuaternion());
	Vector3 pos{ 0.6f, 0.9f, 0.0f };
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, GetOwner()->GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
	ComponentsUpdate(elapsedTime);

	m_isHit = false;
}


void RightArm::Finalize()
{
	
}

void RightArm::Collision(Collider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}