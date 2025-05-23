#include "pch.h"
#include "EnemyBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include <random>

// コンストラクタ
EnemyBullet::EnemyBullet(Scene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	AddComponent<Trail>();
	GetComponent<Collider>()->Initialize(id,Collider::Trigger, { 0.1f,0.1f,0.1f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Cube));
	GetComponent<Trail>()->Initialize(L"Resources/Textures/white.png", 10,DirectX::Colors::Red);
	SetScale({ 0.125f,0.125f,0.125f });
	SetState(BulletState::UNUSED);
}

// デストラクタ
EnemyBullet::~EnemyBullet()
{

}

// 初期化処理
void EnemyBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);
	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);
	GetComponent<Trail>()->ClearBuffer();
	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
	GetComponent<Collider>()->SetActive(false);
}

// 弾を発射
void EnemyBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	GetComponent<Trail>()->ClearBuffer();

	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	GetOwner()->GetQuaternion();

	// ランダムな方向に弾をばらけさせる
	std::random_device seed_gen;
	std::mt19937 mt(seed_gen());
	std::uniform_real_distribution<float> dist(-DIFFUSION, DIFFUSION);
	float resultx = dist(mt);
	float resulty = dist(mt);
	float resultz = dist(mt);

	// プレイヤーの移動先+ランダムな方向を射撃目標にする
	Vector3 predictionPosition = LinePrediction(static_cast<Player*>(target)) + Vector3(resultx, resulty, resultz);

	velocity += Vector3::Transform(Vector3::Backward * SPEED, Quaternion::FromToRotation(GetOwner()->GetPosition(), predictionPosition));

	velocity = predictionPosition - GetOwner()->GetPosition();
	velocity.Normalize();

	SetVelocity(velocity * SPEED);
	SetState(BulletState::FLYING);
	GetComponent<Collider>()->SetActive(true);
}

// 弾が何かに当たった時の処理
void EnemyBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);
	GetComponent<Trail>()->ClearBuffer();
	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
	GetComponent<Collider>()->SetActive(false);
}

// 更新処理
void EnemyBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() + GetVelocity());

	GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0, 0.125f, 0), GetPosition() + Vector3(0, 0.125f, 0));

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}


void EnemyBullet::Render()
{
}

// 当たり判定の処理
void EnemyBullet::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Wall)
	{
		Hit();
	}
}

// プレイヤーの移動方向から射撃座標を計算
DirectX::SimpleMath::Vector3 EnemyBullet::LinePrediction(Player* target)
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

	//虚数解は当たらないので絶対値で無視
	float D = std::sqrt(std::abs(B * B - A * C));
	return target->GetPosition() + targetVel * PlusMin((-B - D) / A, (-B + D) / A);

}

//プラスの最小値を返す(両方マイナスなら0)
float EnemyBullet::PlusMin(float a, float b)
{
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}