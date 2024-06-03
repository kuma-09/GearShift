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

void RightArm::Initialize()
{
}

void RightArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * 2.0f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void RightArm::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::Dice, GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void RightArm::Finalize()
{

}