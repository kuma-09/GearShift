#include "pch.h"
#include "BossHead.h"
#include "Game/Game.h"
#include "Game/Player/Player.h"
#include "Game/GameObject.h"
#include "Game/Object/Bullet/Bullet.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/Move.h"
#include <Game/Components/HP.h>
#include "Game/Components/Look.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/BossEnemy.h"

BossHead::BossHead()
{
	AddComponent<ModelDraw>();
	AddComponent<Look>();

}

BossHead::~BossHead()
{
}

void BossHead::Initialize(GameObject* target)
{
	float scale = 5.0f;
	SetScale({ scale,scale,scale });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BossHead));
	GetComponent<ModelDraw>()->SetRimLithgColor(DirectX::Colors::IndianRed);
	GetComponent<Look>()->SetTarget(this, target);
}

void BossHead::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	Quaternion quaternion = GetOwner()->GetQuaternion();
	Vector3 pos{ 0.0f, 2.0f,0.0f };
	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition() + Vector3::Transform(pos, quaternion));
	SetVelocity(GetOwner()->GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll({0,DirectX::XMConvertToRadians(135),0}) * GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);

	ComponentsUpdate(elapsedTime);

}

void BossHead::Render()
{
}

void BossHead::Finalize()
{
}

void BossHead::Collision(Collider* collider)
{
}

