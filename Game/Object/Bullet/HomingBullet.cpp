#include "pch.h"
#include "HomingBullet.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Particle/Emitter.h"
#include "Game/Player/Player.h"
#include "random"

HomingBullet::HomingBullet(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.1f,0.1f,0.1f });
	GetComponent<ModelDraw>()->Initialize(ModelDraw::Cube);
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

	SetOwner(object);


	SetPosition(Vector3::Zero);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void HomingBullet::Shot(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	std::random_device seed_gen;
	std::mt19937 mt(seed_gen());

	std::uniform_real_distribution<float> distX(-2, 2);
	std::uniform_real_distribution<float> distY(0, 2);
	std::uniform_real_distribution<float> distZ(1, 2);


	float resultx = distX(mt);
	float resulty = distY(mt);
	float resultz = distZ(mt);

	SetTarget(object);
	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	SetQuaternion(GetOwner()->GetQuaternion());
	m_position = GetPosition();
	m_velocity = Vector3::Transform(Vector3(resultx * 10.0f, resulty * 10.0f, resultz * 10.0f),GetQuaternion());
	m_period = 2;

	velocity += Vector3::Transform(Vector3::Forward * SPEED, GetQuaternion());
	SetVelocity(velocity);
	SetState(BulletState::FLYING);
}

void HomingBullet::Shot(GameObject* object, float period)
{
	using namespace DirectX::SimpleMath;

	std::random_device seed_gen;
	std::mt19937 mt(seed_gen());

	std::uniform_real_distribution<float> distX(-2, 2);
	std::uniform_real_distribution<float> distY(0, 2);
	std::uniform_real_distribution<float> distZ(1, 2);


	float resultx = distX(mt);
	float resulty = distY(mt);
	float resultz = distZ(mt);

	SetTarget(object);
	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	SetQuaternion(GetOwner()->GetQuaternion());
	m_position = GetPosition();
	m_velocity = Vector3::Transform(Vector3(resultx * 10.0f, resulty * 10.0f, resultz * 10.0f), GetQuaternion());
	m_period = period;

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

	if (GetState() == FLYING)
	{

		Vector3 acceleration = Vector3::Zero;

		Vector3 diff = GetTarget()->GetPosition() - m_position;

		
		acceleration += (diff - m_velocity * m_period) * 2.f / (m_period * m_period);
		


		m_period -= elapsedTime;

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
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld());
		GetComponent<Emitter>()->Render(GetPosition());
	}

}

void HomingBullet::Collision(BoxCollider* collider)
{
	UNREFERENCED_PARAMETER(collider);
}