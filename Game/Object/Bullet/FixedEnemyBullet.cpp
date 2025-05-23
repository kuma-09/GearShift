#include "pch.h"
#include "FixedEnemyBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

// コンストラクタ
FixedEnemyBullet::FixedEnemyBullet(Scene* scene, Collider::TypeID id)
{
	SetScene(scene);
	SetScale({ 0.5f,0.5f,0.5f });
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	GetComponent<Collider>()->Initialize(id,Collider::Trigger, { 0.5f,0.5f,0.5f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Cube));
}

// デストラクタ
FixedEnemyBullet::~FixedEnemyBullet()
{

}

// 初期化処理
void FixedEnemyBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
	GetComponent<Collider>()->SetActive(false);
}

// 弾を発射
void FixedEnemyBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;
	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	GetOwner()->GetQuaternion();
	SetTarget(target);
	velocity += Vector3::Transform(Vector3::Backward * SPEED, GetOwner()->GetQuaternion());
	SetVelocity(velocity);
	SetState(BulletState::FLYING);
	GetComponent<Collider>()->SetActive(true);
}

// 弾が何かに当たった時の処理
void FixedEnemyBullet::Hit()
{
	using namespace DirectX::SimpleMath;
	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);
	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
	GetComponent<Collider>()->SetActive(false);
}

// 更新処理
void FixedEnemyBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() - GetVelocity());

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void FixedEnemyBullet::Render()
{
}

// 当たり判定の処理
void FixedEnemyBullet::Collision(Collider* collider)
{
	if (GetState() == FLYING)
	{
		if (collider->GetTypeID() == Collider::Wall)
		{
			Hit();
		}
	}
}