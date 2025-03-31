#include "pch.h"
#include "TrainingEnemy.h"
#include <iostream>
#include <algorithm>
#include <Framework/Audio.h>
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Physics.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"
#include "Game/Manager/ObjectManager.h"

// �R���X�g���N�^
TrainingEnemy::TrainingEnemy(IScene* scene)
{
	SetScene(scene);
	SetScale({ 3.0f,3.0f,3.0f });

	// �R���|�[�l���g��ǉ�
	AddComponent<HP>();
	AddComponent<Physics>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
}

// �f�X�g���N�^
TrainingEnemy::~TrainingEnemy()
{
}

// ����������
void TrainingEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// �R���|�[�l���g��������
	GetComponent<HP>()->SetHP(10);
	GetComponent<Physics>()->Initialize();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::TankBody));
	GetComponent<ModelDraw>()->SetRimLithgColor(DirectX::Colors::IndianRed);
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision, { 2,0.5f,3 });
	GetComponent<HPBar>()->Initialize();
	// ���W��������
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

// �X�V����
void TrainingEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �R���|�[�l���g���X�V
	ComponentsUpdate(elapsedTime);

	// ���W�̈ړ�
	SetPosition(GetPosition() + GetVelocity());
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);

	// HP���c���Ă��邩�`�F�b�N
	CheckHP();
}

// �`�揈��
void TrainingEnemy::Render()
{	
}

// �I������
void TrainingEnemy::Finalize()
{
}

// �e�𔭎�
void TrainingEnemy::Shot()
{
}

// �X�e�[�g��ύX
void TrainingEnemy::ChangeState(State* state)
{
}

// �����蔻��̏���
void TrainingEnemy::Collision(Collider* collider)
{
	// �v���C���[�̒e�������������̏���
	if (collider->GetTypeID() == Collider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetPosition());
			bulletObject->Hit();
		}
	}
	// �v���C���[�̃u���[�h�������������̏���
	if (collider->GetTypeID() == Collider::PlayerSword)
	{
		Blade* bulletObject = static_cast<Blade*>(collider->GetOwner());
		if (bulletObject->GetState() == Blade::USING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 5);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetPosition());
			bulletObject->Hit();
		}
		else if (bulletObject->GetState() == Blade::USED)
		{
			bulletObject->Hit();
		}
	}
	// ����ǂɓ����������̏���
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
}

// HP���c���Ă��邩�`�F�b�N
void TrainingEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
