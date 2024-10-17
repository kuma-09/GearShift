#include "pch.h"
#include "Game/Object/Wall/BillA.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/BoxCollider.h"

BillA::BillA(IScene* scene)
{
	SetScene(scene);
	AddComponent<BoxCollider>();
	AddComponent<ModelDraw>();
	GetComponent<BoxCollider>()->SetTypeID(BoxCollider::TypeID::Wall);

}

BillA::~BillA()
{
}

void BillA::Initialize()
{
	using namespace DirectX::SimpleMath;

	GetComponent<ModelDraw>()->Initialize(ModelDraw::BillA);
	SetScale({ 10, 8, 6 });
	GetComponent<BoxCollider>()->SetSize(GetScale());
	GetComponent<BoxCollider>()->GetBoundingBox()->Center = GetPosition();

	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z });
	SetWorld(world);
}

void BillA::Update(float elapsedtime)
{
	UNREFERENCED_PARAMETER(elapsedtime);
}

void BillA::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void BillA::Render()
{
	using namespace DirectX::SimpleMath;

	//GetComponent<BoxCollider>()->Render();
	GetComponent<ModelDraw>()->Render(false);
}