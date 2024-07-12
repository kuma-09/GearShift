#include "pch.h"
#include "LeftArm.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

LeftArm::LeftArm()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

LeftArm::~LeftArm()
{

}

void LeftArm::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::LArm);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void LeftArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * -0.6f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up()    *  0.9f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
}

void LeftArm::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),GetHP() <= 0);
	
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void LeftArm::Finalize()
{

}

void LeftArm::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		SetHP(GetHP() - 1);
	}
}