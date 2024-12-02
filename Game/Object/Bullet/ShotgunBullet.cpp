#include "pch.h"
#include "ShotgunBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/PlayScene.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

#include "Framework/Audio.h"

ShotgunBullet::ShotgunBullet(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	GetComponent<Collider>()->SetTypeID(id);
	GetComponent<Collider>()->SetSize({ 0.5f,0.5f,0.5f });
	SetScale({ 0.25f,0.25f,0.25f });
}

ShotgunBullet::~ShotgunBullet()
{

}

void ShotgunBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void ShotgunBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	SetTarget(target);

	velocity = GetTarget()->GetPosition() - GetPosition();
	velocity.Normalize();

	velocity *= SPEED;

	//velocity += Vector3::Transform(Vector3::Forward * SPEED, GetOwner()->GetQuaternion());

	m_totalTime = 0;

	SetVelocity(velocity);
	SetState(BulletState::FLYING);

}

void ShotgunBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	if (GetState() == BulletState::FLYING)
	{
		static_cast<PlayScene*>(GetOwner()->GetScene())->CreateHitEffect(GetPosition());
		Vector3 velocity = Vector3::Zero;
		SetPosition(Vector3::Zero);
		SetQuaternion(Quaternion::Identity);

		SetVelocity(Vector3::Zero);
		SetState(BulletState::USED);

		Audio::GetInstance()->PlaySoundSE_Hit();
	}
}

void ShotgunBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	//Vector3 velocity = GetPosition() - GetTarget()->GetPosition();
	//velocity.Normalize();
	m_totalTime += elapsedTime;
	if (m_totalTime >= MAX_TIME)
	{
		Hit();
	}

	SetPosition(GetPosition() + GetVelocity() * elapsedTime);

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void ShotgunBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld(), false);
	}
}

void ShotgunBullet::Collision(Collider* collider)
{
	if (GetState() == FLYING)
	{
		if (collider->GetTypeID() == Collider::Wall ||
			collider->GetTypeID() == Collider::Floor)
		{
			Hit();
		}
	}
}
