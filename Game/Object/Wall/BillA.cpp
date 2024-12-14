#include "pch.h"
#include "Game/Object/Wall/BillA.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"

BillA::BillA(IScene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();
	GetComponent<Collider>()->SetTypeID(Collider::TypeID::Wall);

}

BillA::~BillA()
{
}

void BillA::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetBillAModel());
	SetScale({ 10, 8, 6 });
	GetComponent<Collider>()->SetSize(GetScale());
	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}

void BillA::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z });
	SetWorld(world);

	GetComponent<Collider>()->SetSize(GetScale());
	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}

void BillA::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void BillA::Render()
{
	using namespace DirectX::SimpleMath;

	//GetComponent<Collider>()->Render();
	GetComponent<ModelDraw>()->Render();
}