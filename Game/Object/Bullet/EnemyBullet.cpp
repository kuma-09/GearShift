#include "pch.h"
#include "EnemyBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/Collider.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Trail.h"
#include <random>

EnemyBullet::EnemyBullet(IScene* scene, Collider::TypeID id)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	AddComponent<Trail>();
	GetComponent<Collider>()->SetTypeID(id);
	GetComponent<Collider>()->SetSize({ 0.1f,0.1f,0.1f });
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetCubeModel());
	GetComponent<Trail>()->Initialize(L"Resources/Textures/particle.png", 10);
	SetScale({ 0.25f,0.25f,0.25f });
	SetState(BulletState::UNUSED);
}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Initialize(GameObject* object)
{
	using namespace DirectX::SimpleMath;

	SetOwner(object);
	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::UNUSED);
}

void EnemyBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	GetComponent<Trail>()->ClearBuffer();

	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	GetOwner()->GetQuaternion();

	// �����_���ȕ����ɒe���΂炯������
	std::random_device seed_gen;
	std::mt19937 mt(seed_gen());
	std::uniform_real_distribution<float> dist(-DIFFUSION, DIFFUSION);
	float resultx = dist(mt);
	float resulty = dist(mt);
	float resultz = dist(mt);

	// �v���C���[�̈ړ���+�����_���ȕ������ˌ��ڕW�ɂ���
	Vector3 predictionPosition = LinePrediction(static_cast<Player*>(target)) + Vector3(resultx, resulty, resultz);

	velocity += Vector3::Transform(Vector3::Backward * SPEED, Quaternion::FromToRotation(GetOwner()->GetPosition(), predictionPosition));

	velocity = predictionPosition - GetOwner()->GetPosition();
	velocity.Normalize();

	SetVelocity(velocity * SPEED);
	SetState(BulletState::FLYING);
}

void EnemyBullet::Hit()
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(Vector3::Zero);
	SetQuaternion(Quaternion::Identity);

	SetVelocity(Vector3::Zero);
	SetState(BulletState::USED);
}

void EnemyBullet::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetPosition() + GetVelocity());

	GetComponent<Trail>()->SetPos(GetPosition() - Vector3(0, 0.5f, 0), GetPosition() + Vector3(0, 0.5f, 0));

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void EnemyBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld(),false,DirectX::Colors::Red);
		GetComponent<Trail>()->Render(DirectX::Colors::Red);
	}
}

void EnemyBullet::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::Wall)
	{
		Hit();
	}
}

DirectX::SimpleMath::Vector3 EnemyBullet::LinePrediction(Player* target)
{
	using namespace DirectX::SimpleMath;


	// �e�̈ړ����x
	SPEED;

	// �ڕW�̈�t���[���̈ړ����x
	Vector3 targetVel = target->GetPosition() - target->GetPrePosition();

	//�ˌ�����ʒu���猩���ڕW�ʒu
	Vector3 targetPos = target->GetPosition() - GetPosition();

	float A = (targetVel.x * targetVel.x + targetVel.y * targetVel.y + targetVel.z * targetVel.z) - SPEED * SPEED;
	float B = 2 * (targetPos.x * targetVel.x + targetPos.y * targetVel.y + targetPos.z * targetVel.z);
	float C = (targetPos.x * targetPos.x + targetPos.y * targetPos.y + targetPos.z * targetPos.z);

	//0���֎~
	if (A == 0 && B == 0)return target->GetPosition();
	if (A == 0)return target->GetPosition() + targetVel * (-C / B / 2);

	//�������͂ǂ���������Ȃ��̂Ő�Βl�Ŗ�������
	float D = std::sqrt(std::abs(B * B - A * C));
	return target->GetPosition() + targetVel * PlusMin((-B - D) / A, (-B + D) / A);

}

//�v���X�̍ŏ��l��Ԃ�(�����}�C�i�X�Ȃ�0)
float EnemyBullet::PlusMin(float a, float b)
{
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}