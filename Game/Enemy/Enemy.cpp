#include "pch.h"
#include "Enemy.h"
#include <iostream>
#include <algorithm>
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Object/Bullet.h"
#include "Game/PlayScene.h"

#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"



Enemy::Enemy(IScene* scene)
{
	SetScene(scene);

	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	m_bullet = std::make_unique<Bullet>(GetScene(), BoxCollider::TypeID::EnemyBullet);
	
	m_attack = std::make_unique<EnemyAttackState>(this);
	m_move = std::make_unique<EnemyMoveState>(this);

	m_state = m_move.get();
}

Enemy::~Enemy()
{
	RemoveAllComponents();

}

void Enemy::Initialize(GameObject* target)
{
	SetHP(10);
	m_target = target;
	GetComponent<Look>()->SetTarget(this, target);
	GetComponent<ModelDraw>()->Initialize(ModelDraw::Dice);
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Enemy);
	GetComponent<BoxCollider>()->SetSize({0.5f, 0.5f, 0.5f});
	GetComponent<HPBar>()->Initialize();
	m_bullet->Initalize(this);
	m_state->Initialize();
}

void Enemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	m_state->Update(elapsedTime);

	m_bullet->Update(elapsedTime);

	// ���W�̈ړ�
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Enemy::Render()
{
	
	m_bullet->Render();
	m_state->Render();
	if (GetHP() <= 0) return;
	GetComponent<ModelDraw>()->Render(GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Enemy::Finalize()
{
	dynamic_cast<PlayScene*>(GetScene())->RemoveCollider(m_bullet->GetComponent<BoxCollider>());
}

void Enemy::Shot()
{
	m_bullet->Shot(this);
}

void Enemy::ChangeState(State* state)
{
	m_state = state;
	m_state->Initialize();
}

void Enemy::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::PlayerBullet)
	{
		Bullet* bulletObject = static_cast<Bullet*>(collider->GetOwner());
		if (bulletObject->GetState() == Bullet::FLYING)
		{
			SetHP(GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetWorld());
			bulletObject->Hit();
		}
	}
}