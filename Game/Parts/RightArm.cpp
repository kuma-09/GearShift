#include "pch.h"
#include "RightArm.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

RightArm::RightArm()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

RightArm::~RightArm()
{

}

void RightArm::Initialize(int hp)
{
	SetScene(GetOwner()->GetScene());
	SetHP(hp);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void RightArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() *  0.6f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up()    *  0.9f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void RightArm::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);
	if (GetHP() >= 0)
	{
		GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::RArm, GetWorld(),false);
	}
	else
	{
		GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::RArm, GetWorld(),true);
	}
	
	GetComponent<BoxCollider>().lock().get()->Render();
}

void RightArm::Finalize()
{

}