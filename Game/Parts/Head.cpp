#include "pch.h"
#include "Head.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

Head::Head()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

Head::~Head()
{

}

void Head::Initialize()
{
}

void Head::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	ComponentsUpdate(elapsedTime);

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetQuaternion()).Up() * 2.6f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void Head::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::Head, GetWorld());
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void Head::Finalize()
{

}