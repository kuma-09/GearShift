#include "pch.h"
#include "BodyBottom.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

BodyBottom::BodyBottom()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

BodyBottom::~BodyBottom()
{

}

void BodyBottom::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::BodyBottom);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void BodyBottom::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;


	SetPosition(GetOwner()->GetPosition());/*
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up() * 0.5f);*/

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
}

void BodyBottom::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	if (GetHP() >= 0)
	{
		GetComponent<ModelDraw>().lock().get()->Render(GetWorld());
	}
	else
	{
		GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),DirectX::Colors::Black);
	}


	//GetComponent<BoxCollider>().lock().get()->Render();

}

void BodyBottom::Finalize()
{

}

void BodyBottom::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		SetHP(GetHP() - 1);
	}
}