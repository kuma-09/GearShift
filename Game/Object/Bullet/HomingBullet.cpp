#include "pch.h"
#include "HomingBullet.h"

#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/PointLight.h"

#include "Game/PlayScene.h"
#include "Game/Player/Player.h"
#include "random"

#include "Framework/Audio.h"

// コンストラクタ
HomingBullet::HomingBullet(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	AddComponent<Emitter>();
	GetComponent<Collider>()->Initialize(id, Collider::Trigger, { 0.1f,0.1f,0.1f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Cube));
	GetComponent<Emitter>()->Initialize(L"Resources/Textures/whitePuff00.png", 1.0f, 0.025f, 0.3f);
	SetScale({ 0.1f,0.1f,0.1f });
	SetState(BulletState::UNUSED);
}

// デストラクタ
HomingBullet::~HomingBullet()
{
}

// 初期化処理
void HomingBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);


	SetPosition(Vector3::Zero);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
	GetComponent<Collider>()->SetActive(false);
}

// 弾を発射
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
	GetComponent<Collider>()->SetActive(true);
}

// 弾が何かに当たった時の処理
void HomingBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	if (GetState() == BulletState::FLYING)
	{
		Vector3 velocity = Vector3::Zero;
		SetWorld(Matrix::Identity);
		SetQuaternion(Quaternion::Identity);

		SetVelocity(Vector3::Zero);
		SetState(BulletState::USED);

		GetComponent<Collider>()->SetActive(false);
	}
}

// 更新処理
void HomingBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	if (GetState() == FLYING)
	{
		Vector3 acceleration = Vector3::Zero;

		Vector3 diff = GetTarget()->GetPosition() - m_position;

		
		acceleration += (diff - m_velocity * m_period) * 2.f / (m_period * m_period);

		if (acceleration.Length() >= 400.0f)
		{
			acceleration.Normalize();
			acceleration *= 400.0f;
		}

		m_period -= elapsedTime;

		m_velocity += acceleration * elapsedTime;
		m_position += m_velocity * elapsedTime;
		SetPosition(m_position);

		GetComponent<Emitter>()->SetParticle(GetPosition() + DirectX::SimpleMath::Vector3((rand() % 3 - 1) * 0.25f, (rand() % 3 - 1) * 0.25f, (rand() % 3 - 1) * 0.25f));
		Matrix world = Matrix::CreateScale(GetScale());
		world *= Matrix::CreateFromQuaternion(GetQuaternion());
		world *= Matrix::CreateTranslation(GetPosition());
		SetWorld(world);
	}
}

// 描画処理
void HomingBullet::Render()
{
}

// 当たり判定の処理
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