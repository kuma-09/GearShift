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

void LeftArm::Initialize()
{
}

void LeftArm::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Right() * -0.6f
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up()    *  2.2f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void LeftArm::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::LArm, GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void LeftArm::Finalize()
{

}