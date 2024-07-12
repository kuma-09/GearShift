#include "pch.h"
#include "Bullet.h"
#include "Game/Components/BoxCollider.h"

Bullet::Bullet(IScene* scene , BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(id);
	GetComponent<BoxCollider>().lock().get()->SetSize({ 0.25f,0.25f,0.25f });
	SetState(BulletState::UNUSED);
}

Bullet::~Bullet()
{

}

void Bullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	m_owner = object;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void Bullet::Shot(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(object->GetPosition());
	SetQuaternion(object->GetQuaternion());

	velocity += Vector3::Transform(Vector3::Forward, GetQuaternion());
	//velocity += rotate.Forward() * 1.0f;
	SetVelocity(velocity);
	SetState(BulletState::FLYING);
}

void Bullet::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
}

void Bullet::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() + GetVelocity());
}

void Bullet::Render()
{
	GetComponent<BoxCollider>().lock().get()->Render();
}

void Bullet::Collision(BoxCollider* collider)
{

}