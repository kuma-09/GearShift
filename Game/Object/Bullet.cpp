#include "pch.h"
#include "Bullet.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"

Bullet::Bullet(IScene* scene , BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(id);
	GetComponent<BoxCollider>().lock().get()->SetSize({ 0.1f,0.1f,0.1f });
	SetScale({ 0.1f,0.1f,0.1f });
	SetState(BulletState::UNUSED);
}

Bullet::~Bullet()
{

}

void Bullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	m_owner = object;
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::Cube);

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
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() + GetVelocity());

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void Bullet::Render()
{	
	if (m_state == FLYING)
	{
		GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
	}

}

void Bullet::Collision(BoxCollider* collider)
{

}