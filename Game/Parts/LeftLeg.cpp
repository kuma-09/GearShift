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

void LeftLeg::Initialize()
{
}

void LeftLeg::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition()
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * -0.4f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up()    *  1.0f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void LeftLeg::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::LLeg, GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void LeftLeg::Finalize()
{

}