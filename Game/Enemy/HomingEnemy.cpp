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
#include "Game/Object/Bullet/EnemyBullet.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/Object/Sword.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"

HomingEnemy::HomingEnemy(IScene* scene,GameObject* target)
{
	SetScene(scene);
	SetTarget(target);
	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<Collider>();
	AddComponent<HPBar>();

	for (int i = 0; i < BULLET_COUNT; i++)
	{
		m_bullet.emplace_back(std::make_unique<HomingBullet>(GetScene(), Collider::TypeID::EnemyBullet));
	}
	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	SetScale({ 0.05f,0.05f,0.05f });

	m_state = GetMoveState();
}

HomingEnemy::~HomingEnemy()
{
	//RemoveAllComponents();
}

void HomingEnemy::Initialize()
{
	using namespace DirectX::SimpleMath;

	GetComponent<HP>()->SetHP(10);
	GetComponent<Look>()->SetTarget(this, GetTarget());
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDiceModel(),true);
	GetComponent<Collider>()->SetTypeID(Collider::TypeID::Enemy);
	GetComponent<Collider>()->SetSize({1,1,1});
	GetComponent<HPBar>()->Initialize();

	for (auto& bullet : m_bullet)
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

void HomingEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	ComponentsUpdate(elapsedTime);

	m_state->Update(elapsedTime);
	for (auto& bullet : m_bullet)
	{
		bullet->Update(elapsedTime);
	}

	// ç¿ïWÇÃà⁄ìÆ
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void HomingEnemy::CreateShader()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void HomingEnemy::Render()
{
	for (auto& bullet : m_bullet)
	{
		bullet->Render();
	}
	m_state->Render();
	GetComponent<ModelDraw>()->Render();
	GetComponent<HPBar>()->Render(GetPosition());
	// ìñÇΩÇËîªíËÇÃï`âÊ
	//GetComponent<Collider>()->Render();
}

void HomingEnemy::Finalize()
{
	Audio::GetInstance()->PlaySoundSE_Hit();
	for (auto& bullet : m_bullet)
	{
		static_cast<PlayScene*>(GetScene())->RemoveCollider(bullet->GetComponent<Collider>());
	}

}

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

void HomingEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void HomingEnemy::Collision(Collider* collider)
{
	if (collider->GetTypeID() == Collider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetWorld());
			bulletObject->Hit();
		}
	}
	if (collider->GetTypeID() == Collider::PlayerSword)
	{
		Sword* bulletObject = static_cast<Sword*>(collider->GetOwner());
		if (bulletObject->GetState() == Sword::USING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 5);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetWorld());
			bulletObject->Hit();
		}
		else if (bulletObject->GetState() == Sword::USED)
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