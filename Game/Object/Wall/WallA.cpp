#include "pch.h"
#include "Game/Object/Wall/WallA.h"
#include "Game/Components/ModelDraw.h"
#include "Game/Components/Collider.h"

WallA::WallA(IScene* scene)
{
	SetScene(scene);
	AddComponent<Collider>();
	AddComponent<ModelDraw>();

}

WallA::~WallA()
{
}

void WallA::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetBillBModel());
	SetScale({ 4, 8, 4 });
	GetComponent<Collider>()->Initialize(Collider::Wall, GetScale());
	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}

void WallA::Update(float elapsedtime)
{
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(elapsedtime);
	Matrix world = Matrix::Identity;
	world *= Matrix::CreateTranslation(Vector3{ GetPosition().x,GetPosition().y - GetScale().y,GetPosition().z });
	SetWorld(world);

	GetComponent<Collider>()->GetBoundingBox()->Center = GetPosition();
}

void WallA::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow();
}

void WallA::Render()
{
	using namespace DirectX::SimpleMath;

	//GetComponent<Collider>()->Render();
	GetComponent<ModelDraw>()->Render();
}