#include "pch.h"
#include "FixedEnemy.h"
#include <iostream>
#include <algorithm>
#include <Framework/Audio.h>
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Physics.h"
#include "Game/Object/Bullet/FixedEnemyBullet.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/State/FixedAttackState.h"
#include "Game/Manager/ObjectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="scene">�V�[��</param>
/// <param name="target">�^�[�Q�b�g�I�u�W�F�N�g</param>
FixedEnemy::FixedEnemy(IScene* scene,GameObject* target)
{

	SetScene(scene);
	SetTarget(target);
	SetScale({ 3.0f,3.0f,3.0f });

	// �R���|�[�l���g��ǉ�
	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<Physics>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<HPBar>();

	// �e�𐶐�
	m_bullet = std::make_unique<FixedEnemyBullet>(GetScene(), Collider::TypeID::EnemyBullet);
	
	// �X�e�[�g�𐶐�
	SetEnemyAttack(std::make_unique<FixedAttackState>(this));
	m_state = GetAttackState();
}

// �f�X�g���N�^
FixedEnemy::~FixedEnemy()
{
}

// ����������
void FixedEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// �R���|�[�l���g��������
	GetComponent<HP>()->SetHP(10);
	GetComponent<Look>()->Initialize(false,false);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<Physics>()->Initialize();
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::TankBody));
	GetComponent<ModelDraw>()->SetRimLithgColor(DirectX::Colors::IndianRed);
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision, { 2,0.5f,3 });
	GetComponent<HPBar>()->Initialize();
	// �e��������
	m_bullet->Initialize(this);
	// �X�e�[�g��������
	m_state->Initialize();
	// ���W��������
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

// �X�V����
void FixedEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// �X�e�[�g���X�V
	m_state->Update(elapsedTime);
	// �e���X�V
	m_bullet->Update(elapsedTime);
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
void FixedEnemy::Render()
{	
}

// �I������
void FixedEnemy::Finalize()
{
}

// �e�𔭎�
void FixedEnemy::Shot()
{
	m_bullet->Shot(static_cast<Player*>(GetTarget()));
}

// �X�e�[�g��ύX
void FixedEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

// �����蔻��̏���
void FixedEnemy::Collision(Collider* collider)
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
void FixedEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		// HP��������΃I�u�W�F�N�g���폜
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
