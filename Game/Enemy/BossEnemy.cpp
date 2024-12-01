#include "pch.h"
#include "BossEnemy.h"
#include <iostream>
#include <algorithm>
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Physics.h"
#include "Game/Object/Bullet/FixedEnemyBullet.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"

#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"

#include "Game/Parts/BossHead.h"
#include "Game/Parts/BossLeg.h"



BossEnemy::BossEnemy(IScene* scene)
{
	SetScene(scene);

	AddComponent<HP>();
	AddComponent<Physics>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	for (int i = 0; i < MAX_FIXED_BULLET; i++)
	{
		m_fixedBullets.push_back(std::make_unique<FixedEnemyBullet>(GetScene(), BoxCollider::TypeID::EnemyBullet));
	}

	for (int i = 0; i < MAX_HOMING_BULLET; i++)
	{
		m_homingBullets.push_back(std::make_unique<HomingBullet>(GetScene(), BoxCollider::TypeID::EnemyBullet));
	}

	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	SetScale({ 3.0f,3.0f,3.0f });

	m_state = GetMoveState();

}

BossEnemy::~BossEnemy()
{
	//RemoveAllComponents();

}

void BossEnemy::Initialize(GameObject* target)
{
	using namespace DirectX::SimpleMath;


	GetComponent<HP>()->SetHP(10);
	SetTarget(target);
	SetPart(Part::Head, std::make_unique<BossHead>(target));
	SetPart(Part::BodyTop, std::make_unique<BossLeg>());
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Enemy);
	GetComponent<BoxCollider>()->SetSize({ 2,1,3 });
	GetComponent<HPBar>()->Initialize();
	for (auto& bullet : m_fixedBullets)
	{
		bullet->Initialize(this);
	}

	for (auto& bullet : m_homingBullets)
	{
		bullet->Initialize(this);
	}
	m_state->Initialize();
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void BossEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_state->Update(elapsedTime);
	for (auto& bullet : m_fixedBullets)
	{
		bullet->Update(elapsedTime);
	}

	for (auto& bullet : m_homingBullets)
	{
		bullet->Update(elapsedTime);
	}
	
	ComponentsUpdate(elapsedTime);
	UpdateParts(elapsedTime);

	// À•W‚ÌˆÚ“®
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

}

void BossEnemy::CreateShader()
{
	CreateShadows();
}

void BossEnemy::Render()
{
	using namespace DirectX::SimpleMath;

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition() + Vector3{ 0,-0.9f,0 });

	SetWorld(world);

	for (auto& bullet : m_fixedBullets)
	{
		bullet->Render();
	}

	for (auto& bullet : m_homingBullets)
	{
		bullet->Render();
	}
	m_state->Render();
	RenderParts();




	if (GetComponent<HP>()->GetHP() <= 0) return;
	GetComponent<BoxCollider>()->Render();
}

void BossEnemy::Finalize()
{
	//dynamic_cast<PlayScene*>(GetScene())->RemoveCollider(m_bullet->GetComponent<BoxCollider>());
}

void BossEnemy::Shot()
{
	for (auto& bullet : m_fixedBullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}

	for (auto& bullet : m_homingBullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}

}

void BossEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void BossEnemy::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetWorld());
			bulletObject->Hit();
		}
	}
	if (collider->GetTypeID() == BoxCollider::PlayerSword)
	{
		Sword* bulletObject = static_cast<Sword*>(collider->GetOwner());
		if (bulletObject->GetState() == Sword::USING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 5);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetWorld());
			bulletObject->Hit();
		}
	}
	if (collider->GetTypeID() == BoxCollider::Floor ||
		collider->GetTypeID() == BoxCollider::Wall)
	{
		BoxCollider::CheckHit(this, collider->GetOwner());

	}
}