#include "pch.h"
#include "LeftLeg.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

LeftLeg::LeftLeg()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

LeftLeg::~LeftLeg()
{

}

void LeftLeg::Initialize(int hp,IScene* scene)
{
	SetScene(scene);
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::LLeg);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);

}

void LeftLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	Vector3 pos{ 0,-0.4f,1.0f };
	//SetPosition(GetPosition() + Vector3::Transform(pos, GetOwner()->GetQuaternion()));


	SetPosition(GetOwner()->GetPosition()
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * -0.4f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Down()  *  0.1f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void LeftLeg::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),GetHP() <= 0);
	
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void LeftLeg::Finalize()
{

}

void LeftLeg::Collision(BoxCollider* collider)
{
	if (collider->GetTypeID() == BoxCollider::EnemyBullet)
	{
		SetHP(GetHP() - 1);
	}
}