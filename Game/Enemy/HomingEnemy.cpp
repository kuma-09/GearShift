#include "pch.h"
#include "HomingEnemy.h"
#include <iostream>
#include <algorithm>
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Components/HPBar.h"
#include "Game/Object/EnemyBullet.h"
#include "Game/PlayScene.h"

#include "Game/Enemy/State/EnemyAttackState.h"
#include "Game/Enemy/State/EnemyMoveState.h"



HomingEnemy::HomingEnemy(IScene* scene)
{
	SetScene(scene);

	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	m_bullet = std::make_unique<EnemyBullet>(GetScene(), BoxCollider::TypeID::EnemyBullet);
	
	m_attack = std::make_unique<EnemyAttackState>(this);
	m_move = std::make_unique<EnemyMoveState>(this);

	m_state = m_move.get();
}

HomingEnemy::~HomingEnemy()
{
	RemoveAllComponents();

}

void HomingEnemy::Initialize(GameObject* target)
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

void HomingEnemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	m_state->Update(elapsedTime);

	m_bullet->Update(elapsedTime);

	// À•W‚ÌˆÚ“®
	SetPosition(GetPosition() + Vector3::Transform(GetVelocity(), GetQuaternion()));

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void HomingEnemy::Render()
{
	
	m_bullet->Render();
	m_state->Render();
	if (GetHP() <= 0) return;
	GetComponent<ModelDraw>()->Render(GetWorld());

	

	//GetComponent<BoxCollider>().lock().get()->Render();
}

void HomingEnemy::Finalize()
{
	dynamic_cast<PlayScene*>(GetScene())->RemoveCollider(m_bullet->GetComponent<BoxCollider>());
}

void HomingEnemy::Shot()
{
	m_bullet->Shot(static_cast<Player*>(m_target));
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
		EnemyBullet* bulletObject = static_cast<EnemyBullet*>(collider->GetOwner());
		if (bulletObject->GetState() == EnemyBullet::FLYING)
		{
			SetHP(GetHP() - 1);
			static_cast<PlayScene*>(GetScene())->CreateHitParticle(GetWorld());
			bulletObject->Hit();
		}
	}
}