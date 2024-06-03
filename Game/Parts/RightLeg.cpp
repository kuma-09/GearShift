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

void RightLeg::Initialize()
{
}

void RightLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * 2.0f 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up() * -2.0f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void RightLeg::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::Dice, GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void RightLeg::Finalize()
{

}