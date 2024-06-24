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

void Head::Initialize(int hp)
{
	SetScene(GetOwner()->GetScene());
	SetHP(hp);
	GetComponent<ModelDraw>().lock().get()->Initialize(ModelDraw::Head);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void Head::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	SetPosition(GetOwner()->GetPosition() 
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up() * 1.3f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
}

void Head::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);

	GetComponent<ModelDraw>().lock().get()->Render(GetWorld(),GetHP() <= 0);
	
	GetComponent<BoxCollider>().lock().get()->Render();
}

void Head::Finalize()
{

}