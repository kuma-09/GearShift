#include "pch.h"
#include "LaserBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

LaserBullet::LaserBullet(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	SetScale({ 0.5f,0.5f,0.5f });
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	GetComponent<Collider>()->SetTypeID(id);
	GetComponent<Collider>()->SetSize({ 0.5f,0.5f,0.5f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());
}

LaserBullet::~LaserBullet()
{

}

void LaserBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void LaserBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;
	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	m_startPosition = GetPosition();
	GetOwner()->GetQuaternion();
	SetTarget(target);
	velocity += Vector3::Transform(Vector3::Backward * SPEED, GetOwner()->GetQuaternion());
	SetVelocity(velocity);
	SetState(BulletState::FLYING);
}

void LaserBullet::Hit()
{
	using namespace DirectX::SimpleMath;
	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);
	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
}

void LaserBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() - GetVelocity());

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void LaserBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld(), false);
		GetComponent<Collider>()->Render();
	}
}

void LaserBullet::Collision(Collider* collider)
{
	if (GetState() == FLYING)
	{
		if (collider->GetTypeID() == Collider::Wall)
		{
			Hit();
		}
	}
}