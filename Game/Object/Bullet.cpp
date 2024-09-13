#include "pch.h"
#include "Bullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

Bullet::Bullet(IScene* scene , BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.1f,0.1f,0.1f });
	SetScale({ 0.5f,0.5f,0.5f });
	SetState(BulletState::UNUSED);
}

Bullet::~Bullet()
{
	RemoveAllComponents();
}

void Bullet::Initalize(GameObject* object)
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

void Bullet::Shot(Player* target)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(m_owner->GetPosition());
	m_owner->GetQuaternion();

	std::random_device seed_gen;
	std::mt19937 mt (seed_gen());

	std::uniform_real_distribution<float> dist(-DIFFUSION, DIFFUSION);

	float resultx = dist(mt);
	float resulty = dist(mt);
	float resultz = dist(mt);

	Vector3 predictionPosition = LinePrediction(target) + Vector3(resultx,resulty,resultz);
	
	

	velocity += Vector3::Transform(Vector3::Backward * SPEED, Quaternion::FromToRotation(m_owner->GetPosition(), predictionPosition));

	velocity = predictionPosition - m_owner->GetPosition();
	velocity.Normalize();

	SetVelocity(velocity * SPEED);
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
		GetComponent<ModelDraw>()->Render(GetWorld());
	}

}

void Bullet::Collision(BoxCollider* collider)
{

}

DirectX::SimpleMath::Vector3 Bullet::LinePrediction(Player* target)
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

	//0割り禁止処理
	if (A == 0)
	{
		if (B == 0)
		{
			return target->GetPosition();
		}
		else
		{
			return target->GetPosition() + targetVel * (-C / B);
		}
	}

	//弾が当たる時間のフレームを計算する
	float flame1, flame2;
	//二次方程式の解の公式の判別式で分類
	float D = B * B - 4 * A * C;
	if (D > 0)
	{

		float E = sqrt(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//解は2つなので正の数の最小値を使う
		flame1 = PlusMin(flame1, flame2);
	}
	else
	{
		//虚数解
		//当たらないので今の位置を狙う
		flame1 = 0;
	}

	//予想位置を返す
	return target->GetPosition() + targetVel * flame1;
}

//プラスの最小値を返す(両方マイナスなら0)
float Bullet::PlusMin(float a, float b)
{
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}