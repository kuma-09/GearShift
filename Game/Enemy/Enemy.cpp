#include "pch.h"
#include "Enemy.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

Enemy::Enemy()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Enemy);
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{

}

void Enemy::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Enemy::Render()
{
	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::Dice,GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Enemy::Finalize()
{

}