#include "pch.h"
#include "NormalBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/PlayScene.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include <random>

#include "Framework/Audio.h"

NormalBullet::NormalBullet(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	AddComponent<Trail>();
	GetComponent<Collider>()->Initialize(id, { 0.5f,0.5f,0.5f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());
	GetComponent<Trail>()->Initialize(L"Resources/Textures/white.png", 10,DirectX::Colors::Yellow);
	SetScale({ 0.1f,0.1f,0.1f });
}

NormalBullet::~NormalBullet()
{

}

void NormalBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);
	GetComponent<Collider>()->SetActive(false);
	GetComponent<Trail>()->ClearBuffer();
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
	GetComponent<Collider>()->SetActive(true);
}

void NormalBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	if (GetState() == BulletState::FLYING)
	{
		//static_cast<PlayScene*>(GetOwner()->GetScene())->CreateHitEffect(GetPosition());
		Vector3 velocity = Vector3::Zero;
		SetWorld(Matrix::Identity);
		SetQuaternion(Quaternion::Identity);

		SetVelocity(Vector3::Zero);
		SetState(BulletState::USED);

		GetComponent<Trail>()->ClearBuffer();
		GetComponent<Collider>()->SetActive(false);
	}
}

void NormalBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	if (GetState() == FLYING)
	{
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
		if (GetState() == FLYING) GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0, 0.125f, 0), GetPosition() + Vector3(0, 0.125f, 0));
	}


}

void NormalBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render();
		GetComponent<Trail>()->Render();
	}
}

void NormalBullet::Collision(Collider* collider)
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
