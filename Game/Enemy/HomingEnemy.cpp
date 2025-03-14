#include "pch.h"
#include "HomingEnemy.h"
#include <iostream>
#include <algorithm>
#include "Framework/Audio.h"
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"
#include "Game/Components/HPBar.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"

#include "Game/Manager/ObjectManager.h"

// �R���X�g���N�^
HomingEnemy::HomingEnemy(IScene* scene,GameObject* target)
{
	SetScene(scene);
	SetTarget(target);
	SetScale({ 0.05f,0.05f,0.05f });

	// �R���|�[�l���g��ǉ�
	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<HPBar>();

	// �e�𐶐�
	for (int i = 0; i < BULLET_COUNT; i++)
	{
		m_bullet.emplace_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	// �X�e�[�g�𐶐�
	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	m_state = GetMoveState();
}

// �f�X�g���N�^
HomingEnemy::~HomingEnemy()
{
}

// ����������
void HomingEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	// �R���|�[�l���g��������
	GetComponent<HP>()->SetHP(10);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::HomingEnemy), true);
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision);
	GetComponent<HPBar>()->Initialize();

	// �e��������
	for (auto& bullet : m_bullet)
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
void HomingEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �R���|�[�l���g���X�V
	ComponentsUpdate(elapsedTime);
	// �X�e�[�g���X�V
	m_state->Update(elapsedTime);
	// �e���X�V
	for (auto& bullet : m_bullet)
	{
		bullet->Update(elapsedTime);
	}

	// ���W�̈ړ�
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);

	// HP���c���Ă��邩�`�F�b�N
	CheckHP();

}

// �`�揈��
void HomingEnemy::Render()
{
}

// �I������
void HomingEnemy::Finalize()
{
}

// �e�𔭎�
void HomingEnemy::Shot()
{
	for (auto& bullet : m_bullet)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

// �X�e�[�g��ύX
void HomingEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

// �����蔻��̏���
void HomingEnemy::Collision(Collider* collider)
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
		else if (bulletObject->GetState() == Sword::USED)
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
void HomingEnemy::CheckHP()
{
	if (GetComponent<HP>()->GetHP() <= 0)
	{
		// HP��������΃I�u�W�F�N�g���폜
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
		Audio::GetInstance()->PlaySoundSE_Explosion();
	}
}
