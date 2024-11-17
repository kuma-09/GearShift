#include "pch.h"
#include "HomingEnemy.h"
#include <iostream>
#include <algorithm>
#include "Game/Components/HP.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Object/Bullet/EnemyBullet.h"
#include "Game/Object/Bullet/HomingBullet.h"
#include "Game/PlayScene.h"
#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"

HomingEnemy::HomingEnemy(IScene* scene)
{
	SetScene(scene);

	AddComponent<HP>();
	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	m_bullet = std::make_unique<HomingBullet>(GetScene(), BoxCollider::TypeID::EnemyBullet);
	
	SetEnemyAttack(std::make_unique<EnemyAttackState>(this));
	SetEnemyMove(std::make_unique<EnemyMoveState>(this));
	SetScale({ 0.05f,0.05f,0.05f });

	m_state = GetMoveState();
}

HomingEnemy::~HomingEnemy()
{
	//RemoveAllComponents();
}

void HomingEnemy::Initialize(GameObject* target)
{
	using namespace DirectX::SimpleMath;

	SetTarget(target);

	GetComponent<HP>()->SetHP(10);
	GetComponent<Look>()->SetTarget(this, target);
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetDiceModel());
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Enemy);
	GetComponent<BoxCollider>()->SetSize({1,1,1});
	GetComponent<HPBar>()->Initialize();
	m_bullet->Initalize(this);
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
	m_bullet->Update(elapsedTime);

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
	GetComponent<ModelDraw>()->CreateShadow(GetWorld());
}

void HomingEnemy::Render()
{
	
	m_bullet->Render();
	m_state->Render();
	GetComponent<ModelDraw>()->Render(GetWorld(),true);
	
	// ìñÇΩÇËîªíËÇÃï`âÊ
	//GetComponent<BoxCollider>()->Render();
}

void HomingEnemy::Finalize()
{
	dynamic_cast<PlayScene*>(GetScene())->RemoveCollider(m_bullet->GetComponent<BoxCollider>());
}

void HomingEnemy::Shot()
{
	m_bullet->Shot(static_cast<Player*>(GetTarget()));
}

void HomingEnemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void HomingEnemy::Collision(BoxCollider* collider)
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

	if (collider->GetTypeID() == BoxCollider::Floor ||
		collider->GetTypeID() == BoxCollider::Wall)
	{
		BoxCollider::CheckHit(this, collider->GetOwner());
	}
}