#include "pch.h"
#include "HomingBullet.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Particle/Emitter.h"
#include "Game/Player/Player.h"

HomingBullet::HomingBullet(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.1f,0.1f,0.1f });
	SetScale({ 0.1f,0.1f,0.1f });
	SetState(BulletState::UNUSED);
}

HomingBullet::~HomingBullet()
{
	RemoveAllComponents();
}

void HomingBullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	m_owner = object;
	GetComponent<ModelDraw>()->Initialize(ModelDraw::Cube);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void HomingBullet::Shot(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(object->GetPosition());
	SetQuaternion(object->GetQuaternion());
	m_position = GetPosition();
	m_velocity = Vector3::One * 10;
	period = 2;

	velocity += Vector3::Transform(Vector3::Forward * SPEED, GetQuaternion());
	SetVelocity(velocity);
	SetState(BulletState::FLYING);
}

void HomingBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
}

void HomingBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	if (m_state == FLYING)
	{

		Vector3 acceleration = Vector3::Zero;

		GameObject* target = static_cast<Player*>(m_owner)->GetTarget();

		Vector3 diff = target->GetPosition() - m_position;

		
		acceleration += (diff - m_velocity * period) * 2.f / (period * period);
		


		period -= elapsedTime;

		m_velocity += acceleration * elapsedTime;
		m_position += m_velocity * elapsedTime;
		SetPosition(m_position);
	}

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void HomingBullet::Render()
{
	if (m_state == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld());
		GetComponent<Emitter>()->Render(GetPosition());
	}

}

void HomingBullet::Collision(BoxCollider* collider)
{

}