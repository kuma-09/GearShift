#include "pch.h"
#include "FixedEnemy.h"
#include <iostream>
#include <algorithm>
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Components/Gravity.h"
#include "Game/Object/Bullet/FixedEnemyBullet.h"
#include "Game/PlayScene.h"

#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"



FixedEnemy::FixedEnemy(IScene* scene)
{
	SetScene(scene);

	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<Gravity>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	m_bullet = std::make_unique<FixedEnemyBullet>(GetScene(), BoxCollider::TypeID::EnemyBullet);
	
	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	SetScale({ 0.05f,0.05f,0.05f });

	m_state = GetMoveState();
}

FixedEnemy::~FixedEnemy()
{
	//RemoveAllComponents();

}

void FixedEnemy::Initialize(GameObject* target)
{
	GetComponent<HP>()->SetHP(10);
	SetTarget(target);
	GetComponent<Look>()->SetTarget(this, target);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDiceModel());
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Enemy);
	GetComponent<BoxCollider>()->SetSize({1,1,1});
	GetComponent<HPBar>()->Initialize();
	m_bullet->Initalize(this);
	m_state->Initialize();
}

void FixedEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	m_state->Update(elapsedTime);
	ComponentsUpdate(elapsedTime);
	m_bullet->Update(elapsedTime);

}

void FixedEnemy::CreateShader()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void FixedEnemy::Render()
{	
	using namespace DirectX::SimpleMath;

	m_bullet->Render();
	m_state->Render();
	if (GetComponent<HP>()->GetHP() <= 0) return;
	// ���W�̈ړ�
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));
	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
	GetComponent<ModelDraw>()->Render(true);
	GetComponent<BoxCollider>()->Render();
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
		FixedEnemyBullet* bulletObject = static_cast<FixedEnemyBullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			GetComponent<HP>()->SetHP(GetComponent<HP>()->GetHP() - 1);
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