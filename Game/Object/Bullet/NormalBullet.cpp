#include "pch.h"
#include "NormalBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

NormalBullet::NormalBullet(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.5f,0.5f,0.5f });
	SetScale({ 0.5f,0.5f,0.5f });
}

NormalBullet::~NormalBullet()
{

}

void NormalBullet::Initalize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);

	GetComponent<ModelDraw>()->Initialize(ModelDraw::Cube);

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void NormalBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	GetOwner()->GetQuaternion();
	SetTarget(target);

	velocity += Vector3::Transform(Vector3::Forward * SPEED, GetOwner()->GetQuaternion());

	m_totalTime = 0;

	SetVelocity(velocity);
	SetState(BulletState::FLYING);


}

void NormalBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
}

void NormalBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

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
}

void NormalBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld());
	}
}

void NormalBullet::Collision(BoxCollider* collider)
{
	if (GetState() == FLYING)
	{
		if (collider->GetTypeID() == BoxCollider::Wall)
		{
			Hit();
		}
	}
}

DirectX::SimpleMath::Vector3 NormalBullet::LinePrediction(Player* target)
{
	using namespace DirectX::SimpleMath;


	// 弾の移動速度
	SPEED;

	// 目標の一フレームの移動速度
	Vector3 targetVel = target->GetPosition() - target->GetPrePosition();

	//射撃する位置から見た目標位置
	Vector3 targetPos = target->GetPosition() - GetPosition();

	float A = (targetVel.x * targetVel.x + targetVel.y * targetVel.y + targetVel.z * targetVel.z) - SPEED * SPEED;
	float B = 2 * (targetPos.x * targetVel.x + targetPos.y * targetVel.y + targetPos.z * targetVel.z);
	float C = (targetPos.x * targetPos.x + targetPos.y * targetPos.y + targetPos.z * targetPos.z);

	//0割禁止
	if (A == 0 && B == 0)return target->GetPosition();
	if (A == 0)return target->GetPosition() + targetVel * (-C / B / 2);

	//虚数解はどうせ当たらないので絶対値で無視した
	float D = std::sqrt(std::abs(B * B - A * C));
	return target->GetPosition() + targetVel * PlusMin((-B - D) / A, (-B + D) / A);

}

//プラスの最小値を返す(両方マイナスなら0)
float NormalBullet::PlusMin(float a, float b)
{
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}