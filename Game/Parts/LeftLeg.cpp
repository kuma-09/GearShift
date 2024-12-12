#include "pch.h"
#include "LeftLeg.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "Game/Components/Camera.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

LeftLeg::LeftLeg()
{
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
	SetTypeID(Part::LeftLeg);
	m_isHit = false;
}

LeftLeg::~LeftLeg()
{
	Finalize();
}

void LeftLeg::Initialize(int hp,IScene* scene)
{
	UNREFERENCED_PARAMETER(hp);
	SetScene(scene);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetlLegModel());
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/smoke_white.png", 0.3f, 0.1f, 0.3f);
}

void LeftLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ -0.4f,-0.2f,-0.0f };
	Vector3 velocity = Vector3::Zero;
	if (GetOwner()->GetComponent<Move>())
	{
		velocity = GetOwner()->GetComponent<Move>()->GetVelocity() / 3;
	}
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));
	SetVelocity(GetOwner()->GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());

	world *= Matrix::CreateTranslation({ 0,-0.5f,0 });
	world *= Matrix::CreateFromYawPitchRoll({velocity.z,0,-velocity.x});
	world *= Matrix::CreateTranslation({ 0,0.5f,0 });

	world *= Matrix::CreateFromQuaternion(quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

	if (static_cast<Player*>(GetOwner())->GetOnFloor())
	{
		GetComponent<Emitter>()->SetParticle(GetPosition() - DirectX::SimpleMath::Vector3{ 0,1.f,0 });
	}

	m_isHit = false;
}

void LeftLeg::Render()
{

}

void LeftLeg::Finalize()
{
}

void LeftLeg::Collision(Collider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}

void LeftLeg::Action()
{
	static_cast<Player*>(GetOwner())->Shot();
}
