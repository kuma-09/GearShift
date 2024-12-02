#include "pch.h"
#include "HomingBullet.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include "Game/PlayScene.h"
#include "Game/Player/Player.h"
#include "random"

#include "Framework/Audio.h"

HomingBullet::HomingBullet(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
	AddComponent<Trail>();
	GetComponent<Collider>()->SetTypeID(id);
	GetComponent<Collider>()->SetSize({ 0.1f,0.1f,0.1f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/smoke_white_big.png",0.3f,0.01f,0.1f);
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10);
	SetScale({ 0.1f,0.1f,0.1f });
	SetState(BulletState::UNUSED);
}

HomingBullet::~HomingBullet()
{

}

void HomingBullet::Initialize(GameObject* object)
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

	if (GetState() == BulletState::FLYING)
	{
		//static_cast<PlayScene*>(GetOwner()->GetScene())->CreateHitEffect(GetPosition());
		Vector3 velocity = Vector3::Zero;
		SetPosition(Vector3::Zero);
		SetQuaternion(Quaternion::Identity);

		SetVelocity(Vector3::Zero);
		SetState(BulletState::USED);

		Audio::GetInstance()->PlaySoundSE_Hit();
	}
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

		if (acceleration.Length() >= 100.0f)
		{
			acceleration.Normalize();
			acceleration *= 100.0f;
		}

		m_period -= elapsedTime;

		m_velocity += acceleration * elapsedTime;
		m_position += m_velocity * elapsedTime;
		SetPosition(m_position);
	}
	
	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);

	if (GetState() == FLYING) GetComponent<Trail>()->SetPos(GetPosition(), GetPosition() + Vector3(0, 1, 0));
}

void HomingBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld(), false);
		//GetComponent<Trail>()->Render();
		//GetComponent<Emitter>()->Render(GetPosition() + DirectX::SimpleMath::Vector3((rand() % 3 - 1) * 0.25f, (rand() % 3 - 1) * 0.25f, (rand() % 3 - 1) * 0.25f));
	}
	GetComponent<Emitter>()->Render(GetPosition() + DirectX::SimpleMath::Vector3((rand() % 3 - 1) * 0.25f, (rand() % 3 - 1) * 0.25f, (rand() % 3 - 1) * 0.25f));
}

void HomingBullet::Collision(Collider* collider)
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