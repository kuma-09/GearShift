#include "pch.h"
#include "Enemy.h"
#include "Game/Components/ModelDraw.h"

Enemy::Enemy()
{
	AddComponent<ModelDraw>();
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
	GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
}

void Enemy::Finalize()
{

}