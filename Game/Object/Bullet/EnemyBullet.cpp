#include "pch.h"
#include "EnemyBullet.h"
#include "Game/Enemy/Enemy.h"
#include "Game/Player/Player.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/ModelDraw.h"
#include <random>

EnemyBullet::EnemyBullet(IScene* scene, BoxCollider::TypeID id)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(id);
	GetComponent<BoxCollider>()->SetSize({ 0.1f,0.1f,0.1f });
	SetScale({ 0.5f,0.5f,0.5f });
	SetState(BulletState::UNUSED);
}

EnemyBullet::~EnemyBullet()
{
	RemoveAllComponents();
}

void EnemyBullet::Initalize(GameObject* object)
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

void EnemyBullet::Shot(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	Vector3 velocity = Vector3::Zero;
	SetPosition(GetOwner()->GetPosition());
	GetOwner()->GetQuaternion();

	std::random_device seed_gen;
	std::mt19937 mt(seed_gen());

	std::uniform_real_distribution<float> dist(-DIFFUSION, DIFFUSION);

	float resultx = dist(mt);
	float resulty = dist(mt);
	float resultz = dist(mt);

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

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void EnemyBullet::Render()
{
	if (GetState() == FLYING)
	{
		GetComponent<ModelDraw>()->Render(GetWorld());
	}

}

void EnemyBullet::Collision(BoxCollider* collider)
{
	UNREFERENCED_PARAMETER(collider);
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

	//0����֎~����
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

	//�e�������鎞�Ԃ̃t���[�����v�Z����
	float flame1, flame2;
	//�񎟕������̉��̌����̔��ʎ��ŕ���
	float D = B * B - 4 * A * C;
	if (D > 0)
	{

		float E = sqrt(D);
		flame1 = (-B - E) / (2 * A);
		flame2 = (-B + E) / (2 * A);
		//����2�Ȃ̂Ő��̐��̍ŏ��l���g��
		flame1 = PlusMin(flame1, flame2);
	}
	else
	{
		//������
		//������Ȃ��̂ō��̈ʒu��_��
		flame1 = 0;
	}

	//�\�z�ʒu��Ԃ�
	return target->GetPosition() + targetVel * flame1;
}

//�v���X�̍ŏ��l��Ԃ�(�����}�C�i�X�Ȃ�0)
float EnemyBullet::PlusMin(float a, float b)
{
	if (a < 0 && b < 0) return 0;
	if (a < 0) return b;
	if (b < 0) return a;
	return a < b ? a : b;
}