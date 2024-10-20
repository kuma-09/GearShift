#include "pch.h"
#include "FixedEnemyBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

FixedEnemyBullet::FixedEnemyBullet(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.5f,0.5f,0.5f });
	SetScale({ 0.5f,0.5f,0.5f });
}

FixedEnemyBullet::~FixedEnemyBullet()
{

}

void FixedEnemyBullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	GetComponent<ModelDraw>()->Initialize(ModelDraw::Cube);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void FixedEnemyBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	GetOwner()->GetQuaternion();
	SetTarget(target);

	velocity += Vector3::Transform(Vector3::Backward * SPEED, GetOwner()->GetQuaternion());


	SetVelocity(velocity);
	SetState(BulletState::FLYING);


}

void FixedEnemyBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
}

void FixedEnemyBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() - GetVelocity());

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void FixedEnemyBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(false);
	}

}

void FixedEnemyBullet::Collision(BoxCollider* collider)
{
	if (GetState() == FLYING)
	{
		if (collider->GetTypeID() == BoxCollider::Wall)
		{
			Hit();
		}
	}
}