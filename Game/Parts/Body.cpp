#include "pch.h"
#include "Body.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

Body::Body()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

Body::~Body()
{

}

void Body::Initialize()
{
}

void Body::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition());

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Body::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);


	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::Dice, GetWorld());;
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Body::Finalize()
{

}