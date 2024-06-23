#include "pch.h"
#include "BodyBottom.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

BodyBottom::BodyBottom()
{
	AddComponent<ModelDraw>();
	AddComponent<BoxCollider>();
}

BodyBottom::~BodyBottom()
{

}

void BodyBottom::Initialize(int hp)
{
	SetScene(GetOwner()->GetScene());
	SetHP(hp);
	GetComponent<BoxCollider>().lock().get()->SetTypeID(BoxCollider::TypeID::Player);
}

void BodyBottom::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;


	SetPosition(GetOwner()->GetPosition()
		+ Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion()).Up() * 1.25f);

	Matrix world = Matrix::Identity;
	world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetOwner()->GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);

	ComponentsUpdate(elapsedTime);
}

void BodyBottom::Render(DirectX::SimpleMath::Matrix world)
{
	UNREFERENCED_PARAMETER(world);


	GetComponent<ModelDraw>().lock().get()->Render(ModelDraw::BodyBottom, GetWorld());;
	//GetComponent<BoxCollider>().lock().get()->Render();
}

void BodyBottom::Finalize()
{

}