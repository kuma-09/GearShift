#include "pch.h"
#include "Bullet.h"
#include "Game/Components/BoxCollider.h"

Bullet::Bullet()
{
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::PlayerBullet);
}

Bullet::~Bullet()
{

}

void Bullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(object->GetPosition());
	SetQuaternion(object->GetQuaternion());

	Matrix rotate = Matrix::CreateFromQuaternion(GetQuaternion());

	velocity += rotate.Forward() * 1.0f;
	SetVelocity(velocity);
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