#include "pch.h"
#include "FixedEnemy.h"
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



FixedEnemy::FixedEnemy(IScene* scene)
{
	SetScene(scene);

	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<Physics>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	m_bullet = std::make_unique<FixedEnemyBullet>(GetScene(), BoxCollider::TypeID::EnemyBullet);
	
	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	SetScale({ 3.0f,3.0f,3.0f });

	m_state = GetMoveState();
}

FixedEnemy::~FixedEnemy()
{
	//RemoveAllComponents();

}

void FixedEnemy::Initialize(GameObject* target)
{
	using namespace DirectX::SimpleMath;


	GetComponent<HP>()->SetHP(10);
	SetTarget(target);
	GetComponent<Look>()->Initialize(false,true);
	GetComponent<Look>()->SetTarget(this, target);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetTankBodyModel());
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Enemy);
	GetComponent<BoxCollider>()->SetSize({2,1,3});
	GetComponent<HPBar>()->Initialize();
	m_bullet->Initialize(this);
	m_state->Initialize();
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void FixedEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	m_state->Update(elapsedTime);
	m_bullet->Update(elapsedTime);
	ComponentsUpdate(elapsedTime);


	// ç¿ïWÇÃà⁄ìÆ
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

}

void FixedEnemy::CreateShader()
{
	GetComponent<ModelDraw>()->CreateShadow(GetWorld());
}

void FixedEnemy::Render()
{	
	using namespace DirectX::SimpleMath;

	m_bullet->Render();
	m_state->Render();

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition() + Vector3{ 0,-0.9f,0 });

	SetWorld(world);


	if (GetComponent<HP>()->GetHP() <= 0) return;
	GetComponent<ModelDraw>()->Render(GetWorld(), false);
	GetComponent<HPBar>()->Render(GetPosition());
	//GetComponent<BoxCollider>()->Render();
}

void FixedEnemy::Finalize()
{
	dynamic_cast<PlayScene*>(GetScene())->RemoveCollider(m_bullet->GetComponent<BoxCollider>());
}

void FixedEnemy::Shot()
{
	m_bullet->Shot(static_cast<Player*>(GetTarget()));
}

void FixedEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void FixedEnemy::Collision(BoxCollider* collider)
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