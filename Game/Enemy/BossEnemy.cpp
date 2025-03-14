#include "pch.h"
#include "BossEnemy.h"
#include <iostream>
#include <algorithm>
#include "Framework/Audio.h"
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Physics.h"

#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"

#include "Game/Enemy/BossState/BossGatlingState.h"
#include "Game/Enemy/BossState/BossMissileState.h"
#include "Game/Enemy/BossState/BossMoveState.h"

#include "Game/Parts/BossHead.h"
#include "Game/Parts/BossLeg.h"

#include "Game/Manager/ObjectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="scene">�V�[��</param>
/// <param name="target">�^�[�Q�b�g�I�u�W�F�N�g</param>
BossEnemy::BossEnemy(IScene* scene,GameObject* target)
{
	SetScene(scene);
	SetTarget(target);
	SetScale({ 3.0f,3.0f,3.0f });

	// �R���|�[�l���g��ǉ�
	AddComponent<HP>();
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
	AddComponent<Look>();

	// �e�𐶐�
	CreateBullets();
	// �X�e�[�g���쐬
	CreateStates();

}

// �f�X�g���N�^
BossEnemy::~BossEnemy()
{

}

// ����������
void BossEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// �p�[�c�𐶐�
	SetPart(Part::Head, std::make_unique<BossHead>(GetTarget()));
	SetPart(Part::BodyTop, std::make_unique<BossLeg>());

	// �R���|�[�l���g��������
	GetComponent<HP>()->Initialize(25);
	GetComponent<Physics>()->Initialize();
	GetComponent<Look>()->Initialize(false, true);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision, { 6,5,6 });
	GetComponent<HPBar>()->Initialize();

	// �e��������
	for (auto& bullet : m_bullets)
	{
		bullet->Initialize(this);
	}
	for (auto& bullet : m_homingBullets)
	{
		bullet->Initialize(this);
	}

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
void BossEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// �X�e�[�g���X�V
	m_state->Update(elapsedTime);

	CheckHP();

	// �e���X�V
	for (auto& bullet : m_bullets)
	{
		bullet->Update(elapsedTime);
	}
	for (auto& bullet : m_homingBullets)
	{
		bullet->Update(elapsedTime);
	}
	
	// �R���|�[�l���g���X�V
	ComponentsUpdate(elapsedTime);
	// �p�[�c���X�V
	UpdateParts(elapsedTime);

	// ���W�̈ړ�
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition() + Vector3{ 0,-0.9f,0 });

	SetWorld(world);
}

// �`�揈��
void BossEnemy::Render()
{
}

// �I������
void BossEnemy::Finalize()
{
}

// �e�𔭎�
void BossEnemy::Shot()
{
	// �g�p����Ă��Ȃ��e�𔭎�
	for (auto& bullet : m_bullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

// �~�T�C���𔭎�
void BossEnemy::ShotMissile()
{
	// �g�p����Ă��Ȃ��~�T�C���𔭎�
	for (auto& bullet : m_homingBullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

// �K�g�����O�������[�h
void BossEnemy::ReloadGatling()
{
	for (auto& bullet : m_bullets)
	{
		bullet->Initialize(this);
	}
}

// �X�e�[�g��ύX
void BossEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

// �����蔻��̏���
void BossEnemy::Collision(Collider* collider)
{
	// �v���C���[�̒e�������������̏���
	if (collider->GetTypeID() == Collider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(bulletObject->GetPosition());
			bulletObject->Hit();
		}
	}
	// �v���C���[�̃u���[�h�������������̏���
	if (collider->GetTypeID() == Collider::PlayerSword)
	{
		Sword* bulletObject = static_cast<Sword*>(collider->GetOwner());
		if (bulletObject->GetState() == Sword::USING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 5);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(bulletObject->GetPosition());
			bulletObject->Hit();
		}
		else if(bulletObject->GetState() == Sword::USED)
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


// �e���쐬
void BossEnemy::CreateBullets()
{
	for (int i = 0; i < MAX_BULLET; i++)
	{
		m_bullets.push_back(std::make_unique<EnemyBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	for (int i = 0; i < MAX_HOMING_BULLET; i++)
	{
		m_homingBullets.push_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}
}

// �X�e�[�g���쐬
void BossEnemy::CreateStates()
{
	SetEnemyAttack(std::make_unique<BossGatlingState>(this));
	SetEnemyMove(std::make_unique<BossMoveState>(this));
	m_missileState = std::make_unique<BossMissileState>(this);
	m_tackleState = std::make_unique<BossTackleState>(this);
	m_state = GetMoveState();
}

// HP���c���Ă��邩�`�F�b�N
void BossEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		// HP��������΃I�u�W�F�N�g���폜
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
