#include "pch.h"
#include "Enemy.h"
#include "Game/Components/Look.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"
#include "Game/Object/Bullet.h"
#include <iostream>
#include <algorithm>

Enemy::Enemy(IScene* scene)
	:m_totalTime{0}
{
	SetScene(scene);

	AddComponent<Look>();
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();

	
	
}

Enemy::~Enemy()
{

}

void Enemy::Initialize(GameObject* target)
{
	m_bullet = std::make_unique<Bullet>(GetScene(),BoxCollider::TypeID::EnemyBullet);
	GetComponent<Look>().lock().get()->SetTarget(this, target);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::Dice);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Enemy);
}

void Enemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	m_totalTime += elapsedTime;
	if (m_totalTime >= SHOT_INTERVAL)
	{
		m_bullet->Initalize(this);
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
	GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),false);
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Enemy::Finalize()
{

}