#include "pch.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

BillB::BillB(IScene* scene)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Wall);

}

BillB::~BillB()
{
}

void BillB::Initialize()
{
	using namespace DirectX::SimpleMath;

	GetComponent<ModelDraw>()->Initialize(ModelDraw::BillB);
	SetScale({ 4, 8, 4 });
	GetComponent<BoxCollider>()->SetSize(GetScale());
	GetComponent<BoxCollider>()->GetBoundingBox()->Center = GetPosition();

	Matrix world = Matrix::Identity;
	world *= Matrix::CreateScale(GetScale());
	world *= Matrix::CreateTranslation(GetPosition());
	SetWorld(world);
}

void BillB::Update(float elapsedtime)
{
	UNREFERENCED_PARAMETER(elapsedtime);
}

void BillB::Render()
{
	using namespace DirectX::SimpleMath;

	GetComponent<BoxCollider>()->Render();
	GetComponent<ModelDraw>()->Render(Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z }));
}