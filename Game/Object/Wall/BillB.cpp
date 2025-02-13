#include "pch.h"
#include "Game/Object/Wall/BillB.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"

BillB::BillB(IScene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();

}

BillB::~BillB()
{
}

void BillB::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::BillB));
	SetScale({ 4, 8, 4 });
	GetComponent<Collider>()->Initialize(Collider::Wall,Collider::Fixed, GetScale());
	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}

void BillB::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z });
	SetWorld(world);

	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}

void BillB::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void BillB::Render()
{
	using namespace DirectX::SimpleMath;

	//GetComponent<Collider>()->Render();
	GetComponent<ModelDraw>()->Render();
}