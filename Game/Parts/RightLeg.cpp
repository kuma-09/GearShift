#include "pch.h"
#include "RightLeg.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

RightLeg::RightLeg()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

RightLeg::~RightLeg()
{

}

void RightLeg::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::RLeg);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void RightLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * 0.4f 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Down()  * 0.1f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void RightLeg::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);
	
	GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),GetHP() <= 0);
	
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void RightLeg::Finalize()
{

}

void RightLeg::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		SetHP(GetHP() - 1);
	}
}