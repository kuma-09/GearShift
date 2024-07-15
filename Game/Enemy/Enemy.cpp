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



Enemy::Enemy(IScene* scene)
	:m_totalTime{0}
{
	SetScene(scene);

	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	AddComponent<HPBar>();

	m_bullet = std::make_unique<Bullet>(GetScene(), BoxCollider::TypeID::EnemyBullet);
	
}

Enemy::~Enemy()
{
	dynamic_cast<PlayScene*>(GetScene())->RemoveCollider(m_bullet->GetComponent<BoxCollider>().lock().get());
}

void Enemy::Initialize(GameObject* target)
{
	SetHP(10);
	GetComponent<Look>().lock().get()->SetTarget(this, target);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::Dice);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Enemy);
	GetComponent<HPBar>().lock().get()->Initialize();
	m_bullet->Initalize(this);
}

void Enemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	m_totalTime += elapsedTime;
	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_bullet->Shot(this);
		m_totalTime = 0;
	}

	m_bullet->Update(elapsedTime);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Enemy::Render()
{
	
	m_bullet->Render();

	if (GetHP() <= 0) return;
	GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Enemy::Finalize()
{

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