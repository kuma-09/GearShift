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

void Bullet::Update(float elapsedTime)
{
	ComponentsUpdate(elapsedTime);

	SetPosition(GameObject::GetPosition() + GetVelocity());
}

void Bullet::Render()
{
	GetComponent<BoxCollider>().lock().get()->Render();
}