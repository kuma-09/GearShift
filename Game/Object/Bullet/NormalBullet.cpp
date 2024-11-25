#include "pch.h"
#include "NormalBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/PlayScene.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include <random>

#include "Framework/Audio.h"

NormalBullet::NormalBullet(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	AddComponent<Trail>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.5f,0.5f,0.5f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10);
	SetScale({ 0.1f,0.1f,0.1f });
}

NormalBullet::~NormalBullet()
{

}

void NormalBullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void NormalBullet::Shot(GameObject* target)
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

void NormalBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	if (GetState() == BulletState::FLYING)
	{
		//static_cast<PlayScene*>(GetOwner()->GetScene())->CreateHitEffect(GetPosition());
		Vector3 velocity = Vector3::Zero;
		SetPosition(Vector3::Zero);
		SetQuaternion(Quaternion::Identity);

		SetVelocity(Vector3::Zero);
		SetState(BulletState::USED);

		Audio::GetInstance()->PlaySoundSE_Hit();
		GetComponent<Trail>()->ClearBuffer();
	}
}

void NormalBullet::Update(float elapsedTime)
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

	Vector3 pos = { world._41,world._42,world._43 };
	//if (GetState() == FLYING) GetComponent<Trail>()->SetPos(pos, pos + Vector3(0,1,0));
	if (GetState() == FLYING) GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0, 0.5f, 0), GetPosition() + Vector3(0, 0.5f, 0));
}

void NormalBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld(), false);
		GetComponent<Trail>()->Render();
	}
}

void NormalBullet::Collision(BoxCollider* collider)
{
	if (GetState() == FLYING)
	{
		if (collider->GetTypeID() == BoxCollider::Wall ||
			collider->GetTypeID() == BoxCollider::Floor)
		{
			Hit();
		}
	}
}
