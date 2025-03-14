#include "pch.h"
#include "BossLeg.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>
#include "Game/PlayScene.h"

BossLeg::BossLeg()
{
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
}

BossLeg::~BossLeg()
{
}

void BossLeg::Initialize(int hp, IScene* scene)
{
	SetScene(scene);
	float scale = 5.0f;
	SetScale({ scale,scale,scale });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BossLeg));
	GetComponent<ModelDraw>()->SetRimLithgColor(DirectX::Colors::IndianRed);
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/smoke_white.png", 0.1f, 0.1f, 0.3f);
}

void BossLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ 0.0f, -1.0f,0.0f };
	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));
	SetVelocity(GetOwner()->GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(-quaternion);
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);

}

void BossLeg::Render()
{
}

void BossLeg::Finalize()
{
}

void BossLeg::Collision(Collider* collider)
{
}

void BossLeg::Action()
{
}
