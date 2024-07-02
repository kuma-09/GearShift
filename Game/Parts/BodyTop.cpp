#include "pch.h"
#include "BodyTop.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

BodyTop::BodyTop()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

BodyTop::~BodyTop()
{

}


void BodyTop::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::BodyTop);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void BodyTop::Update(float elapsedTime)
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

void BodyTop::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),GetHP() <= 0);


	GetComponent<BoxCollider>().lock().get()->Render();
	
}

void BodyTop::Finalize()
{

}