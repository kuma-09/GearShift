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

BossHead::BossHead(GameObject* target)
{
	AddComponent<HP>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<Emitter>();
	AddComponent<Look>();
	GetComponent<Look>()->SetTarget(this, target);
	m_isHit = false;
}

BossHead::~BossHead()
{
	Finalize();
}

void BossHead::Initialize(int hp, IScene* scene)
{
	SetScene(scene);
	float scale = 5.0f;
	SetScale({ scale,scale,scale });
	GetComponent<HP>()->SetHP(hp);
	SetMaxHP(float(hp));
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BossHead));
	GetComponent<ModelDraw>()->SetRimLithgColor(DirectX::Colors::IndianRed);
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Trigger, { 1,1,1 }, { 0,-0.4f,0 });
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

	m_isHit = false;
}

void BossHead::Render()
{

	GetComponent<ModelDraw>()->Render();
}

void BossHead::Finalize()
{
}

void BossHead::Collision(Collider* collider)
{
	// パーツとの当たり判定
	if (GetComponent<Collider>()->GetBoundingBox()->Intersects(*collider->GetBoundingBox()))
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
				GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
				bulletObject->Hit();
				m_isHit = true;
		}
	}
}

