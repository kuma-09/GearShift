#include "pch.h"
#include "BossEnemy.h"
#include <iostream>
#include <algorithm>
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

BossEnemy::BossEnemy(IScene* scene,GameObject* target)
{
	SetScene(scene);
	SetTarget(target);
	AddComponent<HP>();
	AddComponent<Physics>();
	AddComponent<Collider>();
	AddComponent<HPBar>();
	AddComponent<Look>();
	GetComponent<Look>()->Initialize(false, true);
	GetComponent<Look>()->SetTarget(this, target);

	for (int i = 0; i < MAX_FIXED_BULLET; i++)
	{
		m_fixedBullets.push_back(std::make_unique<EnemyBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	for (int i = 0; i < MAX_HOMING_BULLET; i++)
	{
		m_homingBullets.push_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	for (size_t i = 0; i < MAX_LASER_BULLET; i++)
	{
		m_laserBullet.emplace_back(std::make_unique<LaserBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}

	SetEnemyAttack(std::make_unique<BossGatlingState>(this));
	SetEnemyMove(std::make_unique<BossMoveState>(this));
	m_missileState = std::make_unique<BossMissileState>(this);
	m_tackleState = std::make_unique<BossTackleState>(this);
	SetScale({ 3.0f,3.0f,3.0f });

	m_state = GetMoveState();

}

BossEnemy::~BossEnemy()
{

}

void BossEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;


	GetComponent<HP>()->Initialize(25);
	GetComponent<Physics>()->Initialize();
	SetTarget(GetTarget());
	SetPart(Part::Head, std::make_unique<BossHead>(GetTarget()));
	SetPart(Part::BodyTop, std::make_unique<BossLeg>());
	GetComponent<Collider>()->Initialize(Collider::Enemy,Collider::Collision, { 6,5,6 });
	GetComponent<HPBar>()->Initialize();
	for (auto& bullet : m_fixedBullets)
	{
		bullet->Initialize(this);
	}

	for (auto& bullet : m_homingBullets)
	{
		bullet->Initialize(this);
	}

	for (auto& bullet : m_laserBullet)
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
	
	for (auto& bullet : m_laserBullet)
	{
		bullet->Update(elapsedTime);
	}
	ComponentsUpdate(elapsedTime);
	UpdateParts(elapsedTime);

	// ç¿ïWÇÃà⁄ìÆ
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

	if (GetComponent<HP>()->GetHP() <= 0)
	{
		ObjectManager::Remove(this);
		static_cast<PlayScene*>(GetScene())->CreateHitEffect(GetPosition());
	}

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition() + Vector3{ 0,-0.9f,0 });

	SetWorld(world);
}

void BossEnemy::CreateShader()
{
	CreateShadows();
}

void BossEnemy::Render()
{
	using namespace DirectX::SimpleMath;



	for (auto& bullet : m_fixedBullets)
	{
		bullet->Render();
	}

	for (auto& bullet : m_homingBullets)
	{
		bullet->Render();
	}

	for (auto& bullet : m_laserBullet)
	{
		bullet->Render();
	}
	m_state->Render();
	RenderParts();

	if (GetComponent<HP>()->GetHP() <= 0) return;
	GetComponent<HPBar>()->Render(GetPosition() + Vector3{ 0,5.0f,0 });
	//GetComponent<Collider>()->Render();
}

void BossEnemy::Finalize()
{
}

void BossEnemy::ShotMissile()
{
	for (auto& bullet : m_homingBullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

void BossEnemy::ShotGatling()
{
	for (auto& bullet : m_fixedBullets)
	{
		if (bullet->GetState() == Bullet::BulletState::UNUSED)
		{
			bullet->Shot(static_cast<Player*>(GetTarget()));
			break;
		}
	}
}

void BossEnemy::ReloadGatling()
{
	for (auto& bullet : m_fixedBullets)
	{
		bullet->Initialize(this);
	}
}

void BossEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void BossEnemy::Collision(Collider* collider)
{
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
	if (collider->GetTypeID() == Collider::Floor ||
		collider->GetTypeID() == Collider::Wall)
	{
		Collider::CheckHit(this, collider->GetOwner());
	}
}